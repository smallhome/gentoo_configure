This project compares the performance of LINUX (kernel: 2.6.24-19) and FreeBSD (kernel: 7.1) 
operating systems via their kernels by measuring kernel call, context switch, page allocation, 
file read, etc. operations. All of these operations have uncached and cached variants to 
determine latency variations between cold (i.e. first use) and hot (i.e. repeated) calls.

For movitations, experimental setup, and results pertaining to this experiment, please read 
the following paper: http://www.cs.virginia.edu/~kc5dm/docs/projects/osperf_report2009.pdf

