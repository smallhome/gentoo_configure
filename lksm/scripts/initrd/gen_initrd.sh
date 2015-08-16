#!/bin/bash

cd initrd
find . | cpio -H newc -o > ../bin/initrd.cpio
