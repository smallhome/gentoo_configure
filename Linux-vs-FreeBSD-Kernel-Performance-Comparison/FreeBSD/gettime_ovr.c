/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* standard definitions */
#define LOOP_COUNT 1000

/* forward declaration */
float AverageTime(long T[], int Count);

/* main program */
int main(void)
{
  /* variable declaration */
  int i;
  struct timeval Start, End;
  long T[LOOP_COUNT];
  float At;
  
  /* main loop */
  for (i = 0; i < LOOP_COUNT; i++)
  {
    /* start counter */
    gettimeofday(&Start, NULL);
    
    /* end counter */
    gettimeofday(&End, NULL);
    
    T[i] = End.tv_usec - Start.tv_usec;
  }
   
  /* calculate average time */
  At = AverageTime(T, LOOP_COUNT);
  printf("gettimeofday() overhead (in us) = %f\n", At);
   
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

