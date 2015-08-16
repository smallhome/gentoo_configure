#!/bin/sh
WARMUPS=0
#default REPETITION described in lmbench.8 is 11
REPETITIONS=11

MSG="0 1 2 3 4 5 6 7 8 10 12 16 20 24 32 48 64 96 128 196 256 384 512"

file_dest="./LAT_UNIX_results/"
extension="results.txt"
rm -rf $file_dest
mkdir -p $file_dest
file=$file_dest$extension

for size in $MSG
do      
        RUN="../bin/arm/lat_unix -W $WARMUPS -N $REPETITIONS -m $size"
      	if test -s $file 
		then
		$RUN >> $file 2>&1
	else
		$RUN > $file 2>&1
	fi
done
echo "" 1>&2

cd ./gnuplot_UNIX/
gnuplot -e "load './plot_unix.txt'"
cd ..
