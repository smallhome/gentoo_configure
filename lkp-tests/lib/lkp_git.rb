#!/usr/bin/env ruby

require 'set'
require 'time'
require 'git'

LKP_SRC ||= ENV["LKP_SRC"] || File.dirname(File.dirname File.realpath $PROGRAM_NAME)

GIT_WORK_TREE	||= ENV['GIT_WORK_TREE'] || ENV['LINUX_GIT'] || '/c/repo/linux'
GIT_DIR		||= ENV['GIT_DIR'] || GIT_WORK_TREE + '/.git'
GIT		||= "git --work-tree=#{GIT_WORK_TREE} --git-dir=#{GIT_DIR}"

require "#{LKP_SRC}/lib/yaml.rb"
require "#{LKP_SRC}/lib/simple_cache_method"
require "#{LKP_SRC}/lib/assert"

module Git
	class Base
		include SimpleCacheMethod

		cache_method :gcommit, ->obj {obj.project}
		alias_method :orig_initialize, :initialize

		attr_reader :project

		def initialize(options = {})
			orig_initialize(options)
			@project = options[:project]
		end

		# add tag_names because Base::tags is slow to obtain all tag objects
		# FIXME consider to cache this method
		def tag_names
			lib.tag('-l').split("\n")
		end

		def commit_exist?(commit)
			lib.orig_command('rev-list', ['-1', commit])
		rescue
			false
		else
			true
		end

		def default_remote
			# FIXME remove ENV usage
			# TODO abstract File.dirname(File.dirname logic
			lkp_src = ENV["LKP_SRC"] || File.dirname(File.dirname File.realpath $PROGRAM_NAME)

			load_yaml(lkp_src + "/repo/#{@project}/DEFAULTS")
		end

		def release_tags
			pattern = Regexp.new '^' + default_remote['release_tag_pattern'].sub(' ', '$|^') + '$'
			self.tag_names.select {|tag_name| pattern.match(tag_name)}
		end

		def release_tags_with_order
			pattern = Regexp.new '^' + default_remote['release_tag_pattern'].sub(' ', '$|^') + '$'

			tags = sort_tags(pattern, self.release_tags)

			Hash[tags.map.with_index {|tag, i| [tag, -i]}]
		end

		def release_shas
			release_tags.map {|release_tag| lib.command('rev-list', ['-1', release_tag])}
		end

		def release_tags2shas
			tags = release_tags
			shas = release_shas
			tags2shas = {}
			tags.each_with_index {|tag, i| tags2shas[tag] = shas[i]}
			tags2shas
		end

		def release_shas2tags
			tags = release_tags
			shas = release_shas
			shas2tags = {}
			shas.each_with_index {|sha, i| shas2tags[sha] = tags[i]}
			shas2tags
		end

		cache_method :release_tags, ->obj {obj.project}
		cache_method :release_tags_with_order, ->obj {obj.project}
		cache_method :release_shas, ->obj {obj.project}
		cache_method :release_tags2shas, ->obj {obj.project}
		cache_method :release_shas2tags, ->obj {obj.project}

		def release_tag_order(tag)
			release_tags_with_order[tag]
		end
	end

	class Lib
		def command_lines(cmd, opts = [], chdir = true, redirect = '')
			command_lines = command(cmd, opts, chdir)

			begin
				command_lines.split("\n")
			rescue Exception => e
				# to deal with "GIT error: cat-file ["commit", "9f86262dcc573ca195488de9ec6e4d6d74288ad3"]: invalid byte sequence in US-ASCII"
				# - one possibility is the encoding of string is wrongly set (due to unknown reason), e.g. UTF-8 string's encoding is set as US-ASCII
				#   thus to force_encoding to utf8_string and compare to error check of utf8_string, if same, will consider as wrong encoding info
				utf8_command_lines = command_lines.clone.force_encoding("UTF-8")
				if utf8_command_lines == utf8_command_lines.encode("UTF-8", "UTF-8", undef: :replace)
					utf8_command_lines.split("\n")
				else
					STDERR.puts "GIT error: #{cmd} #{opts}: #{e.message}"
					STDERR.puts "GIT env: LANG = #{ENV['LANG']}, LANGUAGE = #{ENV['LANGUAGE']}, LC_ALL = #{ENV['LC_ALL']}, "\
					            "encoding = #{command_lines.encoding}"

					command_lines.encode("UTF-8", "binary", invalid: :replace, undef: :replace).split("\n")
				end
			end
		end

		public :command_lines
		public :command

		alias_method :orig_command, :command
		def command(cmd, opts = [], chdir = true, redirect = '', &block)
			begin
				orig_command(cmd, opts, chdir, redirect, &block)
			rescue Exception => e
				STDERR.puts "GIT error: #{cmd} #{opts}: #{e.message}"
				STDERR.puts "GIT dump: #{self.inspect}"
				raise
			end
		end
	end

	class Author
		def formatted_name
			"#{@name} <#{@email}>"
		end
	end

	class Object
		class Commit
			include SimpleCacheMethod

			alias_method :orig_initialize, :initialize
			def initialize(base, sha, init = nil)
				orig_initialize(base, sha, init)
				# this is to convert non sha1 40 such as tag name to corresponding commit sha
				# otherwise Object::AbstractObject uses @base.lib.revparse(@objectish) to get sha
				# which sometimes is not as expected when we give a tag name
				self.objectish = @base.lib.command('rev-list', ['-1', self.objectish]) unless Git.sha1_40?(self.objectish)
			end

			def project
				@base.project
			end

			def subject
				self.message.split("\n").first
			end

			# FIXME rli9 need a better name, or remove the function if not common
			def name
				interested_tag || self.sha
			end

			def tags
				@tags ||= @base.lib.tag('--points-at', self.sha).split
			end

			def parent_shas
				@parent_shas ||= self.parents.map {|commit| commit.sha}
			end

			def show(content)
				@base.lib.command_lines('show', "#{self.sha}:#{content}")
			end

			def interested_tag
				@interested_tag ||= release_tag || tags.first
			end

			def release_tag
				unless @release_tag
					release_tags_with_order = @base.release_tags_with_order
					@release_tag = tags.find {|tag| release_tags_with_order.include? tag}
				end
				@release_tag
			end

			#
			# if commit has a version tag, return it directly
			# otherwise checkout commit and get latest version from Makefile.
			#
			def last_release_tag
				return [release_tag, true] if release_tag

				case @base.project
				when 'linux'
					Git.linux_last_release_tag_strategy(@base, self.sha)
				else
					last_release_sha = @base.lib.command("rev-list --first-parent #{self.sha} | grep -m1 -Fx \"#{@base.release_shas.join("\n")}\"").chomp

					last_release_sha.empty? ? nil : [@base.release_shas2tags[last_release_sha], false]
				end
			end

			# v3.11     => v3.11
			# v3.11-rc1 => v3.10
			def last_official_release_tag
				tag, is_exact_match = self.last_release_tag
				return tag unless tag =~ /-rc/

				order = @base.release_tag_order(tag)
				tag_with_order = @base.release_tags_with_order.find {|tag, o| o <= order && tag !~ /-rc/}

				tag_with_order ? tag_with_order[0] : nil
			end

			# v3.11     => v3.10
			# v3.11-rc1 => v3.10
			def prev_official_release_tag
				tag, is_exact_match = self.last_release_tag

				order = @base.release_tag_order(tag)
				tag_with_order = @base.release_tags_with_order.find do |tag, o|
					next if o > order
					next if o == order and is_exact_match

					tag !~ /-rc/
				end

				tag_with_order ? tag_with_order[0] : nil
			end

			# v3.12-rc1 => v3.12
			# v3.12     => v3.13
			def next_official_release_tag
				tag = self.release_tag
				return nil unless tag

				order = @base.release_tag_order(tag)
				@base.release_tags_with_order.reverse_each do |tag, o|
					next if o <= order

					return tag unless tag =~ /-rc/
				end

				nil
			end

			def version_tag
				tag, is_exact_match = last_release_tag

				tag += '+' if tag && !is_exact_match
				tag
			end

			cache_method :last_release_tag, ->(obj) {obj.to_s}
		end

		class Tag
			def commit
				@base.gcommit(@base.lib.command('rev-list', ['-1', @name]))
			end
		end
	end

	class << self
		include SimpleCacheMethod

		# TODO move the ENV evaluation resposibility to caller or another helper function
		# TODO deduce project from branch
		# init a repository
		#
		# options
		#		:project => 'project_name', default is linux
		#		:repository => '/path/to/alt_git_dir', default is '/working_dir/.git'
		#
		# example
		#		Git.init(project: 'dpdk')
		#		Git.init(repository: '/path/to/alt_git_dir')
		#
		alias_method :orig_init, :init
		def init(options = {})
			options[:project] ||= 'linux'

			working_dir = ENV['SRC_ROOT'] || "/c/repo/#{options[:project]}"

			Git.orig_init(working_dir, options)
		end

		#
		# open an existing repository
		#
		alias_method :orig_open, :open
		def open(project)
			assert(project, "project parameter can't be #{project.inspect}")

			working_dir = ENV['SRC_ROOT'] || "/c/repo/#{project}"

			Git.orig_open(working_dir, project: project)
		end

		def linux_last_release_tag_strategy(git_base, commit_sha)
			version = patch_level = sub_level = rc = nil

			git_base.lib.command_lines('show', "#{commit_sha}:Makefile").each do |line|
				case line
				when /VERSION *= *(\d+)/
					version = $1.to_i
				when /PATCHLEVEL *= *(\d+)/
					patch_level = $1.to_i
				when /SUBLEVEL *= *(\d+)/
					sub_level = $1.to_i
				when /EXTRAVERSION *= *-rc(\d+)/
					rc = $1.to_i
				else
					break
				end
			end

			if version && version >= 2
				tag = "v#{version}.#{patch_level}"
				tag += ".#{sub_level}" if version ==2
				tag += "-rc#{rc}" if rc && rc > 0

				[tag, false]
			else
				STDERR.puts "Not a kernel tree? Check #{@base.repo}"
				STDERR.puts caller.join "\n"

				nil
			end
		end

		# FIXME remove ENV usage
		# load remotes information from config files
		#
		# options
		#		:project => 'project_name', default is linux
		#
		def project_remotes(options = {})
			lkp_src = ENV["LKP_SRC"] || File.dirname(File.dirname File.realpath $PROGRAM_NAME)
			options[:project] ||= '*'

			remotes = {}

			Dir[lkp_src + "/repo/#{options[:project]}/*"].each do |file|
				remote = File.basename file
				next if remote == 'DEFAULTS'

				defaults = File.dirname(file) + '/DEFAULTS'
				remotes[remote] = load_yaml_merge [defaults, file]
			end

			remotes
		end

		def sha1_40?(commit)
			commit =~ /^[\da-f]{40}$/
		end

		def commit_name?(commit_name)
			commit_name =~ /^[\da-f~^]{7,}$/ ||
			commit_name =~ /^v[234]\.\d+/ ||
			sha1_40?(commit_name)
		end

		cache_method :project_remotes
		cache_method :init
		cache_method :open
	end
end

def expand_possible_commit(s)
	return s unless Git.commit_name? s

	git = Git.open('linux')
	return s unless git.commit_exist? s
	return git.gcommit(s).sha
end

def is_linus_commit(commit)
	git = Git.open('linux')
	git.gcommit(commit).committer.name == 'Linus Torvalds'
end

def git_commit(commit)
	return commit if Git.sha1_40?(commit)

	$__git_commit_cache ||= {}
	return $__git_commit_cache[commit] if $__git_commit_cache.include?(commit)
	sha1_commit = `#{GIT} rev-list -n1 #{commit}`.chomp
	$__git_commit_cache[commit] = sha1_commit unless sha1_commit.empty?
	return sha1_commit
end

def __commit_tag(commit)
	tags = `#{GIT} tag --points-at #{commit}`.split
	tags.each do |tag|
		return tag if linus_tags.include? tag
	end
	return tags[0]
end

def commit_tag(commit)
	$__commit_tag_cache ||= {}
	return $__commit_tag_cache[commit] if $__commit_tag_cache.include?(commit)
	return $__commit_tag_cache[commit] = __commit_tag(commit)
end

def linus_release_tag(commit)
	return nil unless is_linus_commit(commit)

	tag = commit_tag(commit)
	case tag
	when /^v[34]\.\d+(-rc\d+)?$/, /^v2\.\d+\.\d+(-rc\d)?$/
		tag
	else
		nil
	end
end

# if commit has a version tag, return it directly;
# otherwise checkout commit and get latest version from Makefile.
def __last_linus_release_tag(commit)
	tag = linus_release_tag commit
	return [tag, true] if tag =~ /^v.*/

	version = patch_level = sub_level = rc = nil

	`#{GIT} show #{commit}:Makefile`.each_line { |line|
		case line
		when /VERSION *= *(\d+)/
			version = $1.to_i
		when /PATCHLEVEL *= *(\d+)/
			patch_level = $1.to_i
		when /SUBLEVEL *= *(\d+)/
			sub_level = $1.to_i
		when /EXTRAVERSION *= *-rc(\d+)/
			rc = $1.to_i
		else
			break
		end
	}

	if version and version >= 3
		tag = "v#{version}.#{patch_level}"
	elsif version == 2
		tag = "v2.#{patch_level}.#{sub_level}"
	else
		STDERR.puts "Not a kernel tree? check #{GIT_WORK_TREE}"
		STDERR.puts caller.join "\n"
		return nil
	end

	tag += "-rc#{rc}" if rc and rc > 0
	return [tag, false]
end

def last_linus_release_tag(commit)
	$__last_linus_tag_cache ||= {}
	$__last_linus_tag_cache[commit] ||= __last_linus_release_tag(commit)
	return $__last_linus_tag_cache[commit]
end

def compare_version(aa, bb)
	aa.names.each do |name|
		aaa = aa[name]
		bbb = bb[name]
		next if aaa == bbb
		if name =~ /prerelease/
			direction = -1
		else
			direction = 1
		end
		if aaa and bbb
			unless name =~ /str|name/
				aaa = aaa.to_i
				bbb = bbb.to_i
			end
			return aaa <=> bbb
		elsif aaa and !bbb
			return direction
		elsif !aaa and bbb
			return -direction
		end
	end
	return 0
end

def sort_tags(pattern, tags)
	tags.sort do |a, b|
		aa = pattern.match a
		bb = pattern.match b
		-compare_version(aa, bb)
	end
end

def get_tags(pattern, committer)
	tags = []
	`#{GIT} tag -l`.each_line { |tag|
		tag.chomp!
		next unless pattern.match(tag)
		# disabled: too slow and lots of git lead to OOM
		# next unless committer == nil or committer == git_committer_name(tag)
		tags << tag
	}
	tags
end

# => ["v3.11-rc6", "v3.11-rc5", "v3.11-rc4", "v3.11-rc3", "v3.11-rc2", "v3.11-rc1",
#     "v3.10", "v3.10-rc7", "v3.10-rc6", ..., "v2.6.12-rc3", "v2.6.12-rc2", "v2.6.11"]
def __linus_tags()
	$remotes ||= load_remotes
	pattern = Regexp.new '^' + $remotes['linus']['release_tag_pattern'].sub(' ', '$|^') + '$'
	tags = get_tags(pattern, $remotes['linus']['release_tag_committer'])
	tags = sort_tags(pattern, tags)
	tags_order = {}
	tags.each_with_index do |tag, i|
		tags_order[tag] = -i
	end
	tags_order
end

def linus_tags()
	$__linus_tags_cache ||= __linus_tags
	return $__linus_tags_cache
end

def base_rc_tag(commit)
	commit += '~' if is_linus_commit(commit)
	version, is_exact_match = last_linus_release_tag commit
	return version
end

def load_remotes
	remotes = {}
	files = Dir[LKP_SRC + '/repo/*/*']
	files.each do |file|
		remote = File.basename file
		next if remote == 'DEFAULTS'
		defaults = File.dirname(file) + '/DEFAULTS'
		remotes[remote] = load_yaml_merge [defaults, file]
	end
	remotes
end
