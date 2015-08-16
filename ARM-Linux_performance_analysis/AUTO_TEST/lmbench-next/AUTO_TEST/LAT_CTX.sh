#!/bin/sh
MB=8
WARMUPS=0
REPETITIONS=11

CTX="2 4 8 16 24 32"
N="2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35"

file_dest="./LAT_CTX_results/"
extension="k.txt"


rm -rf $file_dest
mkdir -p $file_dest

set -x
for size in $CTX
do      
	for pnumber in $N
	do
		RUN="../bin/arm/lat_ctx -W $WARMUPS -N $REPETITIONS -s $size $pnumber"

		#flush cache
		sync && echo 3 > /proc/sys/vm/drop_caches

		file=$file_dest$size$extension
		if test -s $file 
			then
			$RUN >> $file 2>&1
		else
			$RUN > $file 2>&1
		fi
	done
done
echo "" 1>&2

cp ./gnuplot_CTX/get_data $file_dest
cd ./LAT_CTX_results/
./get_data
cd ..
cd ./gnuplot_CTX/
sh ./gnuplot_script.sh
cd ..
