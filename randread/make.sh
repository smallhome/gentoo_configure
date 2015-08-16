#!/bin/bash
OS=`uname -s`
ARCH=`uname -p`
if [ "$OS" == "Darwin" ]; then
  CC=clang
else
  CC=gcc
fi

CC_ARGS="-Wall"

if [ "$ARCH" != "unknown" ]; then
  CC_ARGS="$CC_ARGS -m64"
fi

$CC $CC_ARGS randread.c -o randread.$OS-$ARCH 
