#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>

int arr[64 * 1024 * 80] = { 2 };//just for big enough

int test_line(int step)
{
	struct timeval start;
    struct timeval end;

	long unsigned int i = 0;
	gettimeofday(&start, NULL);
	for (; i < (int)(sizeof(arr)/sizeof(int)); i += step) 
	{
		arr[i] *= arr[i];
	}
	gettimeofday(&end, NULL);
	return ( 1000000*(end.tv_sec - start.tv_sec)+ (end.tv_usec - start.tv_usec) ) ;
}


int main(int argc, char *argv[])
{
    int test_step = atoi(argv[1]);
	printf("%d %d\n", test_step, test_line(test_step));
	return 0;
}
