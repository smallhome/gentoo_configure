/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

/* standard definitions */
#define BUFF_COUNT    8 
#define TF            1000000
#define FILE_NAME     "TestFile"
#define EMPTY_STRING  ""
#define FILE_SIZE     67108864
#define OPR_COUNT     2
#define SWAP_SIZE     1073741824
#define BUFF_SIZE     1048576

/* main program */
int main(void)
{
   /* variable declaration */
   uint32_t CCLa, CCHa, CCHb, CCLb;
   uint64_t Fa, Fb, T;
   int FileHandle, Random;
   long long i, j, BytesCount, BytesRead, Read, Count;
   static int NumBuff[] = {4, 16, 64, 256, 1024, 4096, 16384, 65536};
   char *Buffer[BUFF_COUNT], *Source, *SaveSource, *SwapBuffer;
   char TmpBuffer[BUFF_SIZE];
   float At;

   /* open file */
   FileHandle = open(FILE_NAME, O_NONBLOCK | O_RDONLY);
   
   /* main loop */
   for (i = 0; i < BUFF_COUNT; i++)
   {
      /* initialize variables */
      BytesCount = 0;
      Source = NULL;
      SaveSource = NULL;
      Count = 0;
      At = 0.0;

      /* allocate memory */
      Buffer[i] = (char *)malloc(NumBuff[i]);

      /* initialize buffer */
      strcpy(Buffer[i], EMPTY_STRING);

      /* allocate swap-out buffer */
      SwapBuffer = (char *)malloc(SWAP_SIZE);

      /* initialize swap out buffer */
      strcpy(SwapBuffer, EMPTY_STRING);

      /* setup seed for RNG */
      srand(time(NULL));

      /* fill the buffer with random data */
      while (Count < SWAP_SIZE)
      {  
          /* get a random number */
          Random = rand();

          /* convert integer to string */
          sprintf(TmpBuffer, "%d", Random);
          
          /* write random data into swap out buffer */
          SwapBuffer[Count] = TmpBuffer[Random % BUFF_SIZE];
          
          /* update write count */
          Count = Count + 1;

          /* initialize temporary buffer */
          strcpy(TmpBuffer, EMPTY_STRING);

      } /* while () */

      /* memory map file */
      Source = mmap(0, FILE_SIZE, PROT_READ, MAP_SHARED, FileHandle, 0);
      
      /* cache the file for OPR_COUNT times */
      for (j = 0; j < OPR_COUNT; j++)
      {
         /* initialize variables */
         Count = 0;
         SaveSource = Source;
         
         /* check the read count */
         while (Count < FILE_SIZE)
         {
            /* copy mapped file chunks to a buffer */
            memcpy(Buffer[i], Source, NumBuff[i]);

            /* show results */
           // printf("Buffer[%d] = %s\n", i, Buffer[i]);

            /* update source pointer */
            Source++;

            /* update read count */
            Count = Count + NumBuff[i]; 
 
         } /* while () */
        
         /* show result */
         //printf("Round %d of memory-mapping complete\n", j);

         /* rewind the file */
         lseek(FileHandle, 0, SEEK_SET);

         /* restore source pointer */
         Source = SaveSource;
          
      } /* for (j) */

      /* rewind the file again */
      lseek(FileHandle, 0, SEEK_SET);

      /* start counter */
      __asm__ __volatile__ (
                            "xorl %%eax, %%eax\n"
                            "cpuid\n"
                            "rdtsc\n"
                            : "=a" (CCLa), "=d" (CCHa)
                            :
                            : "%ebx", "%ecx");

      /* check if read count is proper */
      while (BytesCount < FILE_SIZE)
      {
        /* copy mapped chunks of file to buffer */
        memcpy(Buffer[i], Source, NumBuff[i]);

        /* update source pointer */
        Source++;
       
        /* update read count */
        BytesCount = BytesCount + NumBuff[i];

      } /* while ()

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

      /* rewind the file again */
      lseek(FileHandle, 0, SEEK_SET);

      /* de-allocate swap buffer */
      free(SwapBuffer);
 
   } /* for(i) */

   /* unmap memory mapped file */
   munmap(Source, FILE_SIZE);

   /* de-allocate memory */
   for (i = 0; i < BUFF_COUNT; i++)
   {

     free(Buffer[i]);
 
   } /* for () */

   /* close file */
   close(FileHandle);

   /* return default value */
   return(0);
    
} /* main () */

