/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>

/* standard definitions */
#define MEM_COUNT     6
#define PAGE_SIZE     4096
#define WORD_SIZE     4

/* main program */
int main(void)
{
   /* variable declaration */
   uint32_t CCLa, CCHa, CCLb, CCHb;
   uint64_t Fa, Fb, T;
   int i, j, k;
   static int NumOfPages[] = {128, 256, 512, 1024, 2048, 4096};
   char *Memory[MEM_COUNT], *DummyAlloc;
   float At;
   
   /* main loop */
   for (i = 0; i < MEM_COUNT; i++)
   {
      /* warm up cache */
      DummyAlloc = (char *)malloc(PAGE_SIZE * MEM_COUNT);

      /* start counter */
      __asm__  __volatile__ (
                             "xorl %%eax, %%eax\n"
                             "cpuid\n"
                             "rdtsc\n"
                             : "=a" (CCLa), "=d" (CCHa)
                             :
                             : "%ebx", "%ecx");
       
      /* allocate memory */
      Memory[i] = (char *)malloc(NumOfPages[i] * PAGE_SIZE);

      /* end counter */
      __asm__ __volatile__ (
                            "xorl %%eax, %%eax\n"
                            "cpuid\n"
                            "rdtsc\n"
                            : "=a" (CCLb), "=d" (CCHb)
                            : 
                            : "%ebx", "%ecx");
      /* update time */
      Fa = (((uint64_t) CCHa << 32) | CCLa);
      Fb = (((uint64_t) CCHb << 32) | CCLb);
      T = (Fb - Fa);
      At = T/1500.096;
      
      /* show time */
      printf("%f\n", At);
 
      /* touch 1-word in every page */
      for (j = 0; j < NumOfPages[i]; j++)
      {
        for (k = 0; k < WORD_SIZE; k++)
        {

           Memory[i][j * PAGE_SIZE + k] = '.';
         //printf("%d at ((%d x %d), %d)\n", i, j, PAGE_SIZE, k);

        } /* for (k) */

      } /* for (j) */

   } /* for(i) */

   /* de-allocate memory */
   for (i = 0; i < MEM_COUNT; i++)
   {

     free(Memory[i]);
 
   } /* for () */

   /* free dummy alloc */
   free(DummyAlloc);

   /* return default value */
   return(0);
    
} /* main () */

