#!/usr/bin/env python
#  -*- mode: python; -*-
#
# AIM7 Benchmark
#

import sys
import os
import time
import fcntl, struct

benchmark_profile_lock_fd = open("/var/lock/benchmark_profile_lock", "w")
benchmark_profile_lock = fcntl.flock(benchmark_profile_lock_fd, fcntl.LOCK_EX)
if (benchmark_profile_lock == None):
    print "\n\n[[CPU control start]]\n\n"

TRACECMD_TIME=180

def cpu_on_2():
    print 'cpu on 2'
    for x in range(0, 2) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd


    for x in range(2, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd

def cpu_on_4():
    for x in range(0, 4) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd


    print 'cpu on 4'
    for x in range(4, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd

def cpu_on_8():
    print 'cpu on 8'
    for x in range(0, 8) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd


    for x in range(8, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    
def cpu_on_15():
    print 'cpu on 15'
    for x in range(0, 15) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(15, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd

def cpu_on_30():
    print 'cpu on 30'
    for x in range(0, 30) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(30, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd

def cpu_on_45():
    print 'cpu on 45'
    for x in range(0, 45) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(45, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd



def cpu_on_60():
    print 'cpu on 60'
    for x in range(0, 60) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(60, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd


def cpu_on_75():
    print 'cpu on 75'
    for x in range(0, 75) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(75, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd

def cpu_on_90():
    print 'cpu on 90'
    for x in range(0, 90) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(90, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd


def cpu_on_105():
    print 'cpu on 105'
    for x in range(0, 105) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd
    for x in range(105, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 0 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd


def cpu_on_120():
    print 'cpu on 120'
    for x in range(0, 120) :
        #/sys/devices/system/cpu/cpuX/online
        cmd = "echo 1 > /sys/devices/system/cpu/cpu" + str(x) + "/online"
        os.system(cmd)
        print cmd

def trace_start():
    print 'trace cmd start'
    cmd = "trace-cmd record -o trace-cmd.dat -e sched:* -e irq:* -e block:* sleep " + str(TRACECMD_TIME)
    os.system(cmd)
    print cmd

def trace_sched_start():
    print 'trace cmd sched start'
    cmd = "trace-cmd record -o trace-cmd_sched.dat -e sched:* sleep " + str(TRACECMD_TIME)
    os.system(cmd)
    print cmd

def trace_irq_start():
    print 'trace cmd irq start'
    cmd = "trace-cmd record -e irq:* sleep " + str(TRACECMD_TIME)
    os.system(cmd)
    print cmd

def trace_block_start():
    print 'trace cmd sched block start'
    cmd = "trace-cmd record -e block:* sleep " + str(TRACECMD_TIME)
    os.system(cmd)
    print cmd

def stop_aim7():
    print 'stop aim7'
    cmd = "/home/kesl/aim7-ramdisk16/slay.sysv -n multitask"
    os.system(cmd)
    print cmd


def clean_aim7():
    print 'clean aim7 tmp directory'
    for x in range(1, 16) :
        if x < 10 :
            cmd = "rm -rf /tmp/aim7_0" + str(x) + "/*"
            print cmd
            os.system(cmd)
        else :
            cmd = "rm -rf /tmp/aim7_" + str(x) + "/*"
            print cmd
            os.system(cmd)

if __name__ == '__main__':
    print "\n\nCPU Control \n\n"

try:
    cmds = sys.argv[1:]
    while True:
        if (len(cmds) == 0):
            input_data = raw_input("benchmark_profile>> ")
            cmds = input_data.split()
            
        for data in cmds[:]:
            if (data == 'Q' or data == 'q' or data == 'quit' or data == 'exit'):
                fcntl.flock(lock_fd,fcntl.LOCK_UN)
                sys.exit(0)
                break;
            elif (data == 'cpu_on_2'):
                cpu_on_2()
            elif (data == 'cpu_on_4'):
                cpu_on_4()
            elif (data == 'cpu_on_8'):
                cpu_on_8()
            elif (data == 'cpu_on_15'):
                cpu_on_15()
            elif (data == 'cpu_on_30'):
                cpu_on_30()
            elif (data == 'cpu_on_45'):
                cpu_on_45()
            elif (data == 'cpu_on_60'):
                cpu_on_60()
            elif (data == 'cpu_on_75'):
                cpu_on_75()
            elif (data == 'cpu_on_90'):
                cpu_on_90()
            elif (data == 'cpu_on_105'):
                cpu_on_105()
            elif (data == 'cpu_on_120'):
                cpu_on_120()
            elif (data == 'stop_aim7'):
                stop_aim7()
            elif (data == 'clean_aim7'):
                clean_aim7()
            elif (data == 'trace_start' or data == '20'):
                trace_start()
            elif (data == 'trace_sched_start' or data == '21'):
                trace_sched_start()
            elif (data == 'trace_irq_start' or data == '22'):
                trace_irq_start()
            elif (data == 'trace_block_start' or data == '23'):
                trace_block_start()
            else:
                print "Invalid command!"
           
            del cmds[0]
	
except:
    print "cpu control be closed"
    print "ctrl+c detection exit~"
    fcntl.flock(benchmark_profile_lock_fd, fcntl.LOCK_UN)
    sys.exit(0)
    
