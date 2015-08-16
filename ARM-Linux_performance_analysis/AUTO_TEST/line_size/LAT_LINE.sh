#!/bin/sh
K="2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 32 64 128 256 512"

file_dest="./LINE_SIZE_results/"
extension="results.txt"

rm -rf $file_dest
mkdir -p $file_dest
file=$file_dest$extension

./test_line 1 > /dev/null 2>&1

for size in $K
do
	RUN="./test_line $size"

	if test -s $file
    		then
		$RUN >> $file 2>&1
	else
		$RUN > $file 2>&1
	fi
done

gnuplot -e "load 'plot.txt'"
