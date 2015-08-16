/* standard header */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* standard definitions */
#define LOOP_COUNT 1000

/* forward declaration */
float MinTime(uint64_t T[], int Count);

/* main program */
int main(void)
{
  /* variable declaration */
  int i;
  uint32_t CCHa, CCLa, CCHb, CCLb, TH, TL;
  uint64_t Fa, Fb, T[LOOP_COUNT];
  float Mt;
  
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
   
  /* calculate min time */
  Mt = MinTime(T, LOOP_COUNT);     
  printf("RDTSC resolution (in Cycles) = %f\n", Mt);
  printf("RDTSC resolution (in us) = %f\n\n", Mt / (1500.096));
   
  return(0);
    
} /* main () */

/* calculate Min */
float MinTime(uint64_t T[], int Count)
{
  /* variable declaration */
  int i;
  float Min;
  
  /* initialize variables */
  Min = 0.0;

  /* main loop */
  for (i = 0; i < Count; i++)
  {
    /* check if array is zero */
    if (T[i] != 0)
    {
      if (Min > (T[i] - T[i-1]))
        Min = (T[i] - T[i-1]);
    }
  }
 
  /* return min */
  return(Min);
}

