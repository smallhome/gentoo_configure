/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

/* standard definitions */
#define LOOP_COUNT 1000
#define TF 1000000

/* forward declaration */
float AverageTime(long T[], int Count);

/* main program */
int main(void)
{
  /* variable declaration */
  int i, ProcessID;
  struct timeval Start, End;
  long T[LOOP_COUNT];
  float At, Overhead = 0.21;
  
  /* main loop */
  for (i = 0; i < LOOP_COUNT; i++)
  {
    /* start counter */
    gettimeofday(&Start, NULL);
    
    /* get parent process id */
    ProcessID = getppid();
    
    /* end counter */
    gettimeofday(&End, NULL);
    
    T[i] = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);
  }
   
  /* calculate average time */
  At = AverageTime(T, LOOP_COUNT);
  printf("%f\n", (At-Overhead));
   
  return(0);
    
} /* main () */

/* calculate average time */
float AverageTime(long T[], int Count)
{
  /* variable declaration */
  int i;
  float Sum, Avg;
  
  /* initialize variables */
  Sum = 0.0;
  Avg = 0.0;

  /* main loop */
  for (i = 0; i < Count; i++)
  { 
    /* check if array element is zero */
    if (T[i] != 0)
    {
      Sum += T[i];
    }
  }
  
  /* calculate average */
  Avg = Sum/Count;  
 
  /* return average */
  return(Avg);
}

