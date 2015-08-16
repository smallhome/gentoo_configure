#!/bin/bash

# Define system parameters
    systemram=`grep MemTotal /proc/meminfo| tr -s ' ' | cut -f2 -d ' '`
    systemramMiB=$((systemram/1024))
	procs=$(grep "physical id" /proc/cpuinfo | sort -u | wc -l)
	pcores=$(grep "cpu cores" /proc/cpuinfo |sort -u |cut -d":" -f2)
	cores=$((procs*pcores))

# Install binaries and tools
	apt-get install iozone3 bonnie++ lsscsi

# Show all volumes mounted and pick one to check
	echo
	echo "Drives detected:"
	lsscsi
    echo
	echo "Please specify disk to partition and format (ie: /dev/sda)"
	echo "WARNING: This will wipe the volume!"
	read disk

# Partition drive
	parted $disk mklabel gpt 
	parted $disk mkpart ext4 0% 100%
	parted $disk align opt 1

# Make filesystem and mount
	mkfs.ext4 /dev/$disk1
	mkdir -p /mnt/diskbench
	mount -o nobarrier /dev/$disk1 /mnt/diskbench



# Flush out caches
	sync
	echo 3 > /proc/sys/vm/drop_caches

# Bonnie++
	bonnie++ -d /mnt/diskbench -s:$((systemramMiB*3)):64k -r$systemramMib -x1 -b -u0 -f -c$cores -q >> ~/bonnie-results.txt
