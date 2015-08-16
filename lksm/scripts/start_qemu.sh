#!/bin/bash

# kernel location
KERNEL_LOCATION="$HOME/github/scalablelinux"
INITRD_LOCATION="$HOME/github/lksm/scripts/initrd"
QEMU_LOCATION="$HOME/gitkesl/manycore_internal/utils/qemu"

_PWD=`pwd`

# make initrd
cd $INITRD_LOCATION && $INITRD_LOCATION/gen_initrd.sh && cd $_PWD

bridge=br0
tap=$(sudo tunctl -u $(whoami) -b)
sudo ip link set $tap up
sleep 1s
sudo brctl addif $bridge $tap

# start qemu
$QEMU_LOCATION/x86_64-softmmu/qemu-system-x86_64 -cpu host -smp 8  -kernel \
    $KERNEL_LOCATION/arch/x86/boot/bzImage -initrd $INITRD_LOCATION/bin/initrd.cpio \
    -nographic -append "console=ttyS0" -enable-kvm \
	-net nic,vlan=0,model=virtio,macaddr=00:16:35:AF:94:4B \
	-net tap,vlan=0,ifname=$tap,script=no,downscript=no 

sudo brctl delif $bridge $tap
sudo ip link set $tap down
sudo tunctl -d $tap
