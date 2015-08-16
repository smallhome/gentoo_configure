/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* standard definitions */
#define PIPE_THRESHOLD 1000

/* forward declaration */
float AverageTime(uint64_t T[], int Count);

/* main program */
int main(void)
{
  /* variable declaration */
  int i, PipeDesc[2];
  char Buffer[4]="BUFF";
  uint32_t CCLa, CCHa, CCLb, CCHb;
  uint64_t Fa, Fb, T[PIPE_THRESHOLD];
  float At;

  /* create pipe */
  pipe(PipeDesc);
  
  /* main loop */
  for (i = 0; i < PIPE_THRESHOLD; i++)
  {
     /* start counter */
     __asm__ __volatile__ (
                            "xorl %%eax, %%eax\n"
                            "cpuid\n"
                            "rdtsc\n"
                            : "=a" (CCLa), "=d" (CCHa)
                            :
                            : "%ebx", "%ecx");

     /* write to pipe and read from it */
     write(PipeDesc[1], Buffer, 4);
     read(PipeDesc[0], Buffer, 4);
     
     /* end counter */
     __asm__ __volatile__ (
                           "xorl %%eax, %%eax\n"
                           "cpuid\n"
                           "rdtsc\n"
                           : "=a" (CCLb), "=d" (CCHb)
                           :
                           : "%ebx", "%ecx");

     /* update cycles */
     Fa = (((uint64_t)CCHa << 32) | CCLa);
     Fb = (((uint64_t)CCHb << 32) | CCLb);
     T[i] = (Fb - Fa); 
   }

   /* calulate average */
   At = AverageTime(T, PIPE_THRESHOLD);
   printf("%f\n", (At/1500.096));

   /* return default value */
   return (0);

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

