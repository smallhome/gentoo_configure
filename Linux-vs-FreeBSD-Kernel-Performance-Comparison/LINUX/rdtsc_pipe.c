/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>

/* standard definitions */
#define PIPE_THRESHOLD 1000

/* global variables */
static int PPipeCount = 0;
static int CPipeCount = 0;

/* forward declaration */
float AverageTime(uint64_t T[], int Count);

/* main program */
void main(int Arg)
{
  /* variable declaration */
  uint32_t CCLa1, CCHa1, CCLb1, CCHb1, CCLa2, CCHa2, CCLb2, CCHb2;
  int i, PID, PipeDesc1[2], PipeDesc2[2];
  char Buffer1[4], Buffer2[4];
  uint64_t Fa1, Fa2, Fb1, Fb2, T1[PIPE_THRESHOLD], T2[PIPE_THRESHOLD];
  float At, At1, At2, Overhead_PIPE = 0.0; /* add oveahead */

  /* mark the boundary of char buffer */
  Buffer1[4] = '\0';
  Buffer2[4] = '\0';
   
  /* create pipes 1 and 2 */
  pipe(PipeDesc1);
  pipe(PipeDesc2);
   
  /* fork a process */
  if ((PID = fork()) != 0) /* parent */
  {
    /* check the parent  count */
    while (PPipeCount < PIPE_THRESHOLD)
    {
      /* start counter */
      __asm__ __volatile__ (
                            "xorl %%eax, %%eax\n"
                            "cpuid\n"
                            "rdtsc\n"
                            : "=a" (CCLa1), "=d" (CCHa1)
                            :
                            : "%ebx", "%ecx");

      /* send to child on pipe1/1, receive from child on pipe2/0 */
      write(PipeDesc2[1], "BUFF", 4);
      read(PipeDesc1[0], Buffer1, 4);
     // printf("Buffer1 = %s\n", Buffer1);

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
      T1[PPipeCount] = (Fb1 - Fa1);

      /* update parent pipe counter */
      PPipeCount = PPipeCount + 1;

    } /* while () */
  } 
  else /* child */
  {
    /* check the child signal count */
    while (CPipeCount < PIPE_THRESHOLD)
    {
     /* start counter */
     __asm__ __volatile__ (
                           "xorl %%eax, %%eax\n"
                           "cpuid\n"
                           "rdtsc\n"
                           : "=a" (CCLa2), "=d" (CCHa2)
                           :
                           : "%ebx", "%ecx");

     /* receive on pipe 1 - 0 and send on pipe 2 - 1 */
     read(PipeDesc2[0], Buffer2, 4);
     write(PipeDesc1[1], "FFUB", 4);
    // printf("Buffer2 = %s\n", Buffer2);
      
     /* end counter */
     __asm__ __volatile__ (
                           "xorl %%eax, %%eax\n"
                           "cpuid\n"
                           "rdtsc\n"
                           : "=a" (CCLb2), "=d" (CCHb2)
                           :
                           : "%ebx", "%ecx");
      
     /* update cycles */
     Fa2 = (((uint64_t) CCHa2 << 32) | CCLa2);
     Fb2 = (((uint64_t) CCHb2 << 32) | CCLb2);
     T2[CPipeCount] = (Fb2 - Fa2);
       
     /* update child pipe counter */
     CPipeCount = CPipeCount + 1;

    } /* while () */

  } /* if () */
  
  /* check if pipe count has reached PIPE_THRESHOLD */
  if (PPipeCount == PIPE_THRESHOLD)
  {
     /* calculate average for parent */
     At1 = AverageTime(T1, PIPE_THRESHOLD);
  
     /* calculate average for child */
     At2 = AverageTime(T2, PIPE_THRESHOLD);

     /* combine average, with over-head adjustment */
     At = (((At1 + At2)/(2*1500.096)) - Overhead_PIPE);

     /* show average */
     printf("%f\n", At);
  } 
    
} /* main () */

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

