#!/bin/bash

function err () {
	echo -e "$@"
	exit 127
}

ADB="../adb-linux"
DIR="bak$(date +%y%m%d_%H:%M.%S)"

mkdir $DIR
cd $DIR

sudo $ADB kill-server
sudo $ADB start-server
#$ADB wait-for-device
sleep 2

for i in persist persist-lg sns mpt tombstones firmware; do
	echo "Unmounting $i"
	$ADB shell "umount /$i"
done

for i in cache system data; do
	echo "Saving $i"
	$ADB shell "stty raw; cd /; tar -c $i 2> /dev/null" | pv | pbzip2 -9 > $i.tbz&&
	TEST1=$($ADB shell "stty raw; cd /; tar -c $i 2> /dev/null" | pv | pbzip2 -9 | md5sum -b | cut -f1 -d " ")
	[ $? -eq 0 ] || err "$i.tbz archive file was not created properly\nNon-zero exist status"
	[ $(head -n 9999 $i.tbz | wc -c) -ge 9999 ] || err "$i.tbz archive file was not created properly"
	TEST2=$(md5sum -b $i.tbz | cut -f1 -d " ")
	[ $TEST1 == $TEST2 ] || err "$i archive doesn't match md5sum\n$TEST1\n$TEST2"
done

for i in modem modemst1 modemst2 sbl1 sbl2 sbl3 aboot boot recovery drm encrypt fota fsg grow misc mpt pad persist reserved rpm rpmbak sns ssd tombstones tz tzbak; do
	echo "Saving $i"
	$ADB shell "stty raw; cd /dev/block/platform/msm_sdcc.1/by-name; cat $i" | pv > $i
	TEST1=$($ADB shell "stty raw; cd /dev/block/platform/msm_sdcc.1/by-name; cat $i" | pv | md5sum -b | cut -f1 -d " ")
	[ -s $i ] || err "$i file was not created properly"
	TEST2=$(md5sum -b $i | cut -f1 -d " ")
	[ $TEST1 == $TEST2 ] || err "$i file doesn't match md5sum\n$TEST1\n$TEST2"
done
