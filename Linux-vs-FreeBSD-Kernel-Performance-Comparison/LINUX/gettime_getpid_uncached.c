/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

/* standard definitions */
#define TF 1000000

/* main program */
int main(void)
{
  /* variable declaration */
  int i, ProcessID;
  struct timeval Start, End;
  long T;
  float Overhead = 0.21;
  
  /* start counter */
  gettimeofday(&Start, NULL);

  /* get process id */
  ProcessID = getpid();
    
  /* end counter */
  gettimeofday(&End, NULL);
    
  T = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);
   
  printf("%f\n", (T-Overhead));
   
  return(0);
    
} /* main () */

