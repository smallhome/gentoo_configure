/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* standard definitions */
#define LOOP_COUNT 1000

/* forward declaration */
float MinTime(long T[], int Count);

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
   
  /* calculate min time */
  At = MinTime(T, LOOP_COUNT);
  printf("gettimeofday() resolution (in us) = %f\n", At);
   
  return(0);
    
} /* main () */

/* calculate min time */
float MinTime(long T[], int Count)
{
  /* variable declaration */
  int i;
  float Min;
  
  /* initialize variables */
  Min = 0.0;

  /* main loop */
  for (i = 0; i < Count-1; i++)
  { 
    /* check if array element is zero */
    if (T[i] != 0)
    {
      if (Min > (T[i+1] - T[i]))
       Min = T[i+1] - T[i];
    }
  }
  
  /* return average */
  return(-1*Min);
}

