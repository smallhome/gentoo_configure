/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>

/* standard definitions */
#define SIGNAL_THRESHOLD 1000
#define TF 1000000

/* global variables */
static int SignalCount = 0;

/* forward declaration */
void SignalHandler(int Arg);
float AverageTime(long T[], int Count);

/* main program */
void main(int Arg)
{
  /* variable declaration */
  struct timeval Start, End;
  long T[SIGNAL_THRESHOLD];
  float At;
  
  /* start counter */
  gettimeofday(&Start, NULL);
  
  /* setup SIGUSR1 signal handler */
  signal(SIGUSR1, SignalHandler);

  /* check for signal count */
  if (SignalCount > SIGNAL_THRESHOLD)
  {
     /* calculate average */
     At = AverageTime(T, SIGNAL_THRESHOLD);
     printf("%f\n", At);
     return;
  }

  /* raise the signal - SIGUSR1 */
  raise(SIGUSR1);

  /* end counter */
  gettimeofday(&End, NULL);

  /* update time */
  T[SignalCount] = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);

  /* show signal count */
  //printf("SignalCount = %d\n",SignalCount);
    
  /* update signal count */
  SignalCount = SignalCount + 1;

} /* main () */

/* signal handler */
void SignalHandler(int Arg)
{
  /* setup signal handler for SIGUSR2 */
  signal(SIGUSR2, main);

  /* raise signal - SIGUSR2 */
  raise(SIGUSR2);
 
}

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

