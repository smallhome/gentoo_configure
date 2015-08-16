#!/bin/bash

function err () {
	echo -e "$@"
	exit 127
}

ADB="../adb-linux"
DIR="$@"
PORT="60679"

cd $DIR

sudo $ADB kill-server
sudo $ADB start-server
#$ADB wait-for-device
sleep 2

$ADB forward tcp:$PORT tcp:$PORT || err "Could not forward ports to android device"
echo "erasing /data"
$ADB shell "rm -r /data/*"

sleep 2
for i in persist persist-lg sns mpt tombstones firmware; do
	echo "Unmounting $i"
	$ADB shell "umount /$i"
done

for i in cache data system; do
	echo "pushing $i"
	$ADB shell "cd /; /system/xbin/busybox nc -l -p $PORT | busybox tar -x"&
	sleep 1
	pbzip2 -dc "$i.tbz" | busybox nc localhost $PORT || err "Error pushing $i"
done

for i in modem modemst1 modemst2 sbl1 sbl2 sbl3 aboot boot recovery drm encrypt fota fsg grow misc mpt pad persist reserved rpm rpmbak sns ssd tombstones tz tzbak; do
	echo "pushing $i"
	$ADB push "$i" "/dev/block/platform/msm_sdcc.1/by-name/$i" || err "Erro pushing $i"
done
