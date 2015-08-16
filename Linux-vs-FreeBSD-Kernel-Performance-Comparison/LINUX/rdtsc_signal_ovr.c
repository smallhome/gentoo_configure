/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <signal.h>

/* standard definitions */
#define SIGNAL_THRESHOLD 1000

/* global variables */
static int SignalCount = 0;

/* forward declaration */
void SignalHandler(int Arg);
float AverageTime(uint64_t T[], int Count);

/* main program */
void main(int Arg)
{
  /* variable declaration */
  uint32_t CCLa, CCHa, CCLb, CCHb;
  uint64_t Fa, Fb, T[SIGNAL_THRESHOLD];
  float At;
  
  /* start counter */
  __asm__ __volatile__ (
                        "xorl %%eax, %%eax\n"
                        "cpuid\n"
                        "rdtsc\n"
                        : "=a" (CCLa), "=d" (CCHa)
                        :
                        : "%ebx", "%ecx");
  
  /* setup SIGUSR1 signal handler */
  signal(SIGUSR1, SignalHandler);

  /* check for signal count */
  if (SignalCount > SIGNAL_THRESHOLD)
  {
     /* calculate average */
     At = AverageTime(T, SIGNAL_THRESHOLD);
     printf("%f\n", (At/1500.096));
     return;
  }

  /* raise the signal - SIGUSR1 */
  raise(SIGUSR1);

  /* end counter */
  __asm__ __volatile__ (
                        "xorl %%eax, %%eax\n"
                        "cpuid\n"
                        "rdtsc\n"
                        : "=a" (CCLb), "=d" (CCHb)
                        : 
                        : "%ebx", "%ecx");

  /* update time */
  Fa = (((uint64_t)CCHa << 32) | CCLa);
  Fb = (((uint64_t)CCHb << 32) | CCLb);
  T[SignalCount] = (Fb - Fa);

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
float AverageTime(uint64_t T[], int Count)
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

