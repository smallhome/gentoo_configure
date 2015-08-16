/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>

/* standard definitions */
#define BUFF_COUNT    8 
#define TF            1000000
#define FILE_NAME     "TestFile"
#define EMPTY_STRING  ""
#define FILE_SIZE     67108864
#define OPR_COUNT     2

/* main program */
int main(void)
{
   /* variable declaration */
   struct timeval Start, End;
   int i, j, BytesCount, BytesRead, Read, Count, FileHandle;
   static int NumBuff[] = {4, 16, 64, 256, 1024, 4096, 16384, 65536};
   char *Buffer[BUFF_COUNT], *Source, *SaveSource;
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
      At = 0.0;

      /* allocate memory */
      Buffer[i] = (char *)malloc(NumBuff[i]);

      /* initialize buffer */
      strcpy(Buffer[i], EMPTY_STRING);

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
      gettimeofday(&Start, NULL);

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
      gettimeofday(&End, NULL);
      
      /* update time */
      At = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);
      
      /* show time */
      printf("%f\n", At);

      /* rewind the file again */
      lseek(FileHandle, 0, SEEK_SET);
 
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

