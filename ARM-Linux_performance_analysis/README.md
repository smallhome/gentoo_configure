Prerequisites
============================
Before using this tools, we should set up Beaglebone Black(BBB) Environment first.

And Beaglebone Black needs Network environment.

+ [Building for BeagleBone](http://elinux.org/Building_for_BeagleBone)

+ [Building BBB Kernel](http://elinux.org/Building_BBB_Kernel)

+ Python

	$ opkg install python

+ [Gnuplot](https://hackpad.com/Install-gnuplot-on-the-beaglebone-black-oCP8XM60OC5)
	
+ lmbench

	+ Compile lmbench on PC and copy the directory to your BBB.

###
	git clone https://github.com/el8/lmbench-next.git
	
	cd lmbench-next
	
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
 
 
+ Network file system or SSH

	+ You can use NFS or SSH to comunicate with BBB.

IO-bound test environment
=========================

Please go to the following link to download the dblp.xml.

This file is used to test the IO-bound process about the context switch's timeslices.

http://dblp.uni-trier.de/xml/

Please put the file in the AUTO_TEST/IO-bound/ directory.


Building and Verification
=========================

make

./auto
	

