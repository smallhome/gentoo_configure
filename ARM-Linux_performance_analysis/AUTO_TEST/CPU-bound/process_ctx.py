import sys
import io
import re #regular expression
import math

process = sys.argv[1]
filename_input = sys.argv[2]
filename_output = sys.argv[3]

fiboo_finish = 0
fiboo_name = 0
timestamp1 = 0
timestamp2 = 0
time = 1

file_fiboo = open(filename_input, 'r')
file_time_slice = open(filename_output, 'w')

while True:
    line = file_fiboo.readline()
    if not line: break
	
    index_s = line.find(process)
    index_e = index_s + len(process)

    if line[index_s: index_e] == process and line[48:60] == "sched_switch" and fiboo_finish == 0:
	    time_start = index_s+28
	    time_end =  time_start + 10
	    timestamp1 = line[time_start:time_end]
	    fiboo_finish += 1
    elif line[index_s : index_e] == process and line[48:60] == "sched_switch" and fiboo_finish == 1:
	    time_start = index_s+28
	    time_end =  time_start + 10
	    timestamp2 = line[time_start:time_end]
	    index_e = index_e+4
	    fiboo_name = line[index_s:index_e]
	    print('timestamp_start is:' + timestamp1)
	    print(fiboo_name + 'timestamp_end is:' + timestamp2)
	    time_slice = float(timestamp2) - float(timestamp1)
	    print('time slice = PI = %.20f' % time_slice)
	    file_time_slice.write(fiboo_name + ' %d' % time +' %.20f' % time_slice + '\n')
	    time += 1
	    timestamp1 = timestamp2
	    

file_fiboo.close()
file_time_slice.close()
