#!/bin/bash

result="CPU_results"
rm -rf $result
mkdir -p $result

echo "0 0 0 0" > /proc/sys/kernel/printk                                                                          
echo 0 > /sys/kernel/debug/tracing/tracing_on
echo 0 > /sys/kernel/debug/tracing/trace
echo sys_sched_rr_get_interval get_rr_interval_idle get_rr_interval_fair get_rr_interval_rt get_rr_interval_stop > /sys/kernel/debug/tracing/set_ftrace_filter
cat /sys/kernel/debug/tracing/set_ftrace_filter
echo nop > /sys/kernel/debug/tracing/current_tracer
echo 1 > /sys/kernel/debug/tracing/events/sched/enable
echo 0 > /sys/kernel/debug/tracing/events/syscalls/enable
echo 1 > /sys/kernel/debug/tracing/tracing_on
nohup ./matrix_mul &
pid1=$!
nohup ./matrix_mul &
pid2=$!
wait $pid1 $pid2
echo 0 > /sys/kernel/debug/tracing/tracing_on
cat /sys/kernel/debug/tracing/trace > /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs.txt
process="matrix_mul-"
python process_ctx.py $process /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs.txt /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_p.txt
process1="$process$pid1"
process2="$process$pid2"
grep $process1 /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_p.txt > /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_$pid1.txt
grep $process2 /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_p.txt > /AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_$pid2.txt
gnuplot -e "filename1='/AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_$pid1.txt' ; filename2='/AUTO_TEST/CPU-bound/CPU_results/matrix_cfs_$pid2.txt' ; outputfile='/AUTO_TEST/CPU-bound/CPU_results/cpu_cfs.png'" run.gp
