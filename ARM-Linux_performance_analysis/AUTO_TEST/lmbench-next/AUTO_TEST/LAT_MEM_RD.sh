#!/bin/sh
WARMUPS=0
REPETITIONS=11
N="16 32 64 128 256 512 1024"
MB=1M
file="./LAT_MEM_RD_results"
name=".txt"
rm -rf $file
mkdir -p $file

set -x
for size in $N
do 
	sync && echo 3 > /proc/sys/vm/drop_caches
	../bin/arm/lat_mem_rd \
		-W $WARMUPS -N $REPETITIONS $MB $size >& $file/$size$name	
done

gnuplot -e "load './gnuplot_MEM_RD/plot_MEM_RD.txt'"
