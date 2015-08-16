/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

/* standard definitions */
#define MEM_COUNT     6
#define PAGE_SIZE     4096
#define WORD_SIZE     4
#define TF            1000000

/* main program */
int main(void)
{
   /* variable declaration */
   struct timeval Start, End;
   int i, j, k;
   static int NumOfPages[] = {128, 256, 512, 1024, 2048, 4096};
   char *Memory[MEM_COUNT];
   float At;
   
   /* main loop */
   for (i = 0; i < MEM_COUNT; i++)
   {
      /* allocate memory */
      Memory[i] = (char *)malloc(NumOfPages[i] * PAGE_SIZE);

      /* start counter */
      gettimeofday(&Start, NULL);
      
      /* touch 1-word in every page */
      for (j = 0; j < NumOfPages[i]; j++)
      {
        for (k = 0; k < WORD_SIZE; k++)
        {

           Memory[i][j * PAGE_SIZE + k] = '.';
         //printf("%d at ((%d x %d), %d)\n", i, j, PAGE_SIZE, k);

        } /* for (k) */

      } /* for (j) */

      /* end counter */
      gettimeofday(&End, NULL);

      /* update time */
      At = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);

      /* show  result */
      printf("%f\n", At);

   } /* for(i) */

   /* de-allocate memory */
   for (i = 0; i < MEM_COUNT; i++)
   {

     free(Memory[i]);
 
   } /* for () */

   /* return default value */
   return(0);
    
} /* main () */

