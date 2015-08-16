
write-linux-image
======
script to (erase and) create partitions on specified hdd device, then copy master image to first partition

I wrote this bash script to "automate" the creation of the partitions and writing the master image into a hard disk.  I have to write a master image which was created using a normal install of linux mint mate, updated, and made into an "oem" by installing the oem-config package and running oem-config-prepare.

The script expects a symlink named latest.img which should point to the master image.

The second script, `do_chroot`, lets you make changes (all via cli) to the image.  You can also pass a directory name, so all changes are diverted to the directory name.  I usually use this to install a package, and get the `*.deb` to do the actual install in another machine.  Makes getting all the necessary `*.deb` files easy for installing on an offline machine.

## Download
* [Version 0.1](https://github.com/azuer88/write-linux-image/archive/master.zip)
* Other Versions

## Software dependencies
Requires pv, install with: `sudo apt-get install pv`

Requires aufs-tools, if you use the mount.aufs functionality.
```
sudo apt-get install aufs-tools
```


## Usage
I usually have a folder where I store my images, let's call /master_images/.  Inside this folder, I clone this repository (repo), among others.

* change to the correct folder, replace /master_images/ with the your folder name
```
$ cd /master_iamges
```

* clone the repository
```
$ git clone https://github.com/azuer88/write-linux-image.git
```

* link to the script from the repo, we'll be using write_image as our command
```
$ ln -s write-linux-image/write_linux_image write_image
```

* create a link to the image you want to write.  As an example, we'll use linux-image.img as the actual image file, replace with your own.
```
$ ln -s linux-image.img latest.img
```

* assuming that /dev/sdb is the target device, make sure all its partitions are unmounted (not ejected, just unmounted), execute write_image:
```
$ ./write_image /dev/sdb
```

You now have a cloned image in /dev/sdb, where sdb1 is the system partition mounted as root, sdb5 is the swap partition, and sdb6 is the home partition (mounted as /home, obviously).

You can make changes to the image by running do_chroot script:

(1) make the mount point for the image:
```
$ mkdir root
```

(2) link to the actual script:
```
$ ln -s write_linux_image/do_chroot .
```

(3) call the script:
```
./do_chroot
```

You can do most of your customization now, and then just `exit` when done.  You may want to clean up by runnint `apt-get clean` before you `exit`.

For example, you may want to upgrade, then exit:
```
$ apt-get update 
$ apt-get -y upgrade
$ apt-get clean
$ exit
```

* enlarging the disk image size to 6544MiB (6144 + 400), don't use the symbolic link, use the actual filename.
```
$ truncate linux-image.img -s 6544M
$ e2fsck -f linux-image.img
$ resize2fs linux-image.img
```

* shrinking the image size to 6144MiB, don't use the symlink, use the actual filename.
```
$ e2fsck -f linux-image.img
$ resize2fs linux-image.img 6144M
$ truncate linux-image.img -s 6144MB
$ e2fsck -f linux-image.img
```



## Contributors

### Contributors on GitHub
* [Contributors](https://github.com/azuer88/write-linux-image/graphs/contributors)

## License 
* see [LICENSE](https://github.com/azuer88/write-linux-image/blob/master/LICENSE) file

## Version 
* Version X.Y


