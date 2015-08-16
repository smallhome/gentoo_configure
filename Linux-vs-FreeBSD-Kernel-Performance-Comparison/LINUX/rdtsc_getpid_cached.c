/* standard header */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* standard definitions */
#define LOOP_COUNT 1000

/* forward declaration */
float AverageTime(uint64_t T[], int Count);

/* main program */
int main(void)
{
  /* variable declaration */
  int i, ProcessID;
  uint32_t CCHa, CCLa, CCHb, CCLb, TH, TL;
  uint64_t Fa, Fb, T[LOOP_COUNT];
  float At, Overhead = 0.0413;
  
  /* main loop */
  for (i = 0; i < LOOP_COUNT; i++)
  {
    /* start counter */
    __asm__ __volatile__ (
                          "xorl %%eax, %%eax\n"
                          "cpuid\n"
                          "rdtsc\n"
                          : "=a" (CCLa), "=d" (CCHa)
                          :
                          : "%ebx", "%ecx");
     
    /* get process id */
    ProcessID = getpid();
     
    /* end counter */
    __asm__ __volatile__ (
                          "xorl %%eax, %%eax\n"
                          "cpuid\n"
                          "rdtsc\n"
                          : "=a" (CCLb), "=d" (CCHb)
                          :
                          : "%ebx", "%ecx");
     
    /* cycle count: start, end */
    Fa = (((uint64_t)CCHa << 32) | CCLa);
    Fb = (((uint64_t)CCHb << 32) | CCLb);
    
    /* total elapsed cycles */
    T[i] = (Fb - Fa);
  }
   
  /* calculate average */
  At = AverageTime(T, LOOP_COUNT);     
  printf("%f\n", ((At/(1500.096))-Overhead));
   
  return(0);
    
} /* main () */

/* calculate average */
float AverageTime(uint64_t T[], int Count)
{
  /* variable declaration */
  int i;
  float Sum, Average;
  
  /* initialize variables */
  Sum = 0.0;
  Average = 0.0;
  
  /* main loop */
  for (i = 0; i < Count; i++)
  {
    /* check if array element is zero */
    if (T[i] != 0)
    {
      Sum += T[i];
    }
  }
 
  /* compute average */  
  Average = Sum/Count;
  
  /* return average */
  return(Average);
}

