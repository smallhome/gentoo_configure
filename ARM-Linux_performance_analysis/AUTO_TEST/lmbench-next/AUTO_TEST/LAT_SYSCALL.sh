#!/bin/sh

echo -e "[Test the syscall latency. Please wait...]"
WARMUPS=0
REPETITIONS=11

file_dest="./LAT_SYSCALL_results/"
extension1="getppid_log.txt"
extension2="open_log.txt"
extension3="read_log.txt"
extension4="write_log.txt"

rm -rf $file_dest
mkdir -p $file_dest

for i in `seq 1 50`
do
	
	RUN="../bin/arm/lat_syscall \
 	-W $WARMUPS -N $REPETITIONS getppid"
	
	file=$file_dest$extension1
	if test -s $file
		then
		$RUN >> $file 2>&1
	else
		$RUN > $file 2>&1

	fi
done

#lat_syscall open
for i in `seq 1 50`
do
        RUN="../bin/arm/lat_syscall \
        -W $WARMUPS -N $REPETITIONS open"

        file=$file_dest$extension2
        if test -s $file
                then
                $RUN >> $file 2>&1
        else
                $RUN > $file 2>&1

        fi
done

#lat_syscall read
for i in `seq 1 50`
do
        RUN="../bin/arm/lat_syscall \
        -W $WARMUPS -N $REPETITIONS read"

        file=$file_dest$extension3
        if test -s $file
                then
                $RUN >> $file 2>&1
        else
                $RUN > $file 2>&1

        fi
done

#lat_syscall write
for i in `seq 1 50`
do
        RUN="../bin/arm/lat_syscall \
        -W $WARMUPS -N $REPETITIONS write"

        file=$file_dest$extension4
        if test -s $file
                then
                $RUN >> $file 2>&1
        else
                $RUN > $file 2>&1

        fi
done

echo -e "[Test the syscall latency is done.]"
echo -e "[Start draw the picture.]"
cd ./gnuplot_SYS_LAT
gnuplot -e "load './plot_syscall.txt'"
cd ..
echo -e "[gnuplot picture is done.The all results are in the <LAT_SYSCALL_results> ]"
