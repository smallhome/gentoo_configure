/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <signal.h>

/* standard definitions */
#define SIGNAL_THRESHOLD 1000

/* global variables */
static int PSigCount = 0;
static int CSigCount = 0;

/* forward declaration */
void PSigHandler(int Arg);
void CSigHandler(int Arg);
float AverageTime(uint64_t T[], int Count);

/* main program */
void main(int Arg)
{
  /* variable declaration */
  uint32_t CCLa1, CCHa1, CCLb1, CCHb1, CCLa2, CCHa2, CCLb2, CCHb2;
  uint64_t Fa1, Fb1, Fa2, Fb2, T1[SIGNAL_THRESHOLD], T2[SIGNAL_THRESHOLD];
  int PID, PPID;
  float At, At1, At2, Overhead_KILL = 3.10;
   
  /* fork a process */
  if ((PID = fork()) != 0) /* parent */
  {
    /* setup child signal handler */
    signal(SIGUSR2, CSigHandler);
    
    /* check the parent signal count */
    while (PSigCount < SIGNAL_THRESHOLD)
    {
      /* start counter */
      __asm__ __volatile__ (
                            "xorl %%eax, %%eax\n"
                            "cpuid\n"
                            "rdtsc\n"
                            : "=a" (CCLa1), "=d" (CCHa1)
                            : 
                            : "%ebx", "%ecx");

      /* show signal count */
     // printf("Parent Signal Count = %d\n", PSigCount);
     
      /* send SIGUSR1 signal to child */
      kill(PID, SIGUSR1);

      /* end counter */
      __asm__ __volatile__ (
                            "xorl %%eax, %%eax\n"
                            "cpuid\n"
                            "rdtsc\n"
                            : "=a" (CCLb1), "=d" (CCHb1)
                            :
                            : "%ebx", "%ecx");

      /* update cycles */
      Fa1 = (((uint64_t)CCHa1 << 32) | CCLa1);
      Fb1 = (((uint64_t)CCHb1 << 32) | CCLb1);
      T1[PSigCount] = (Fb1 - Fa1);

      /* update parent signal counter */
      PSigCount = PSigCount + 1;

    } /* while () */
  } 
  else /* child */
  {
    /* get process id of parent */
    PPID = getppid();
    
    /* setup parent signal handler */
    signal(SIGUSR1, PSigHandler);

    /* check the child signal count */
    while (CSigCount < SIGNAL_THRESHOLD)
    {
     /* start counter */
     __asm__ __volatile__ (
                           "xorl %%eax, %%eax\n"
                           "cpuid\n"
                           "rdtsc\n"
                           : "=a" (CCLa2), "=d" (CCHa2)
                           : 
                           : "%ebx", "%ecx");

     /* show signal count */
    // printf("Child Signal Count = %d\n", CSigCount);
     
     /* sleep for 1 second  to synchronize, parent and child */
     sleep(1);
      
     /* send SIGUSR2 signal to parent */
     kill(PPID, SIGUSR2);
      
     /* end counter */
     __asm__ __volatile__ (
                           "xorl %%eax, %%eax\n"
                           "cpuid\n"
                           "rdtsc\n"
                           : "=a" (CCLb2), "=d" (CCHb2)
                           :
                           : "%ebx", "%ecx");
      
     /* update cycles */
     Fa2 = (((uint64_t)CCHa2 << 32) | CCLa2);
     Fb2 = (((uint64_t)CCHb2 << 32) | CCLb2);
     T2[CSigCount] = (Fb2 - Fa2);
       
     /* update child signal counter */
     CSigCount = CSigCount + 1;

    } /* while () */

  } /* if () */
  
  /* check if signal count has reached SIGNAL_THRESHOLD */
  if (PSigCount == SIGNAL_THRESHOLD)
  {
     /* calculate average for parent */
     At1 = AverageTime(T1, SIGNAL_THRESHOLD);
  
     /* calculate average for child */
     At2 = AverageTime(T2, SIGNAL_THRESHOLD);

     /* combine average, with over-head adjustment */
     At = (((At1 + At2)/(2*1500.096)) - Overhead_KILL);

     /* show average */
     printf("%f\n", At);
  } 
    
} /* main () */

/* parent signal handler */
void PSigHandler(int Atg)
{
 // printf("Got Signal from Parent\n");
}

/* child signal handler */
void CSigHandler(int Arg)
{ 
 // printf("Got Signal from Child\n");
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

