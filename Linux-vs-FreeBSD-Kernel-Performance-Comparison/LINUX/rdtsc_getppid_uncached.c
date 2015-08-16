/* standard header */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* standard definitions */
#define LOOP_COUNT 1000

/* main program */
int main(void)
{
  /* variable declaration */
  int i, ProcessID;
  uint32_t CCHa, CCLa, CCHb, CCLb, TH, TL;
  uint64_t Fa, Fb, T;
  float Overhead = 0.0413;
  
  /* start counter */
  __asm__ __volatile__ (
                          "xorl %%eax, %%eax\n"
                          "cpuid\n"
                          "rdtsc\n"
                          : "=a" (CCLa), "=d" (CCHa)
                          :
                          : "%ebx", "%ecx");
     
  /* get parent process id */
  ProcessID = getppid();
     
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
  T = (Fb - Fa);
  printf("%f\n", ((T/(1500.096))-Overhead));
   
  return(0);
    
} /* main () */

