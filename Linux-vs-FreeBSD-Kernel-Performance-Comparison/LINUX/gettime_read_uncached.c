/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
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
   struct timeval Start, End;
   int FileHandle, Random;
   long long i, j, BytesCount, BytesRead, Read, Write, Count;
   static int NumBuff[] = {4, 16, 64, 256, 1024, 4096, 16384, 65536};
   char *Buffer[BUFF_COUNT], *SwapBuffer, TmpBuffer[BUFF_SIZE];
   float At;

   /* open file */
   FileHandle = open(FILE_NAME, O_NONBLOCK | O_RDONLY);
   
   /* main loop */
   for (i = 0; i < BUFF_COUNT; i++)
   {
      /* initialize variables */
      BytesRead = 0;
      BytesCount = 0;
      Count = 0;
      Read = 0;
      Write = 0;
      At = 0.0;

      /* allocate memory */
      Buffer[i] = (char *)malloc(NumBuff[i]);

      /* initialize buffer */
      strcpy(Buffer[i], EMPTY_STRING);

      /* allocate swap-out buffer */
      SwapBuffer = (char *)malloc(SWAP_SIZE);

      /* initialize swap out buffer */
      strcpy(SwapBuffer, EMPTY_STRING);

      /* setup seed of RNG */
      srand(time(NULL)); 
      
      /* cached the file for OPR_COUNT times */
      for (j = 0; j < OPR_COUNT; j++)
      {
          /* initialize variables */
          Read = 0;
          Count = 0;
          
           /* check the read count */
           while (Count < FILE_SIZE)
           {
              /* read from the file */
              Read = read(FileHandle, Buffer[i], NumBuff[i]);
              
              /* update read count */
              Count = Count + Read; 

           } /* while () */
       
           /* rewind the file */
           lseek(FileHandle, 0, SEEK_SET);

      } /* for (j) */

      /* fill the buffer with random data */
      while (Count < SWAP_SIZE)
      {
        /* get random number */
        Random = rand();

        /* convert integer to string */
        sprintf(TmpBuffer, "%d", Random);

        /* write data into swap out buffer */
        SwapBuffer[Count] = TmpBuffer[Random % BUFF_SIZE];

        /* update write count */
        Count = Count + 1;

        /* initialize buffer */
        strcpy(TmpBuffer, EMPTY_STRING);

        /* show result */
        //printf("Written = %d bytes\n", Count);

      } /* while () */

      /* show result */
      //printf("Swap-out buffer size = %d\n", SWAP_SIZE);
        
      /* start counter */
      gettimeofday(&Start, NULL);

      /* check if read count is proper */
      while (BytesCount < FILE_SIZE)
      {
        /* read the file */
        BytesRead = read(FileHandle, Buffer[i], NumBuff[i]);
       
        /* update read count */
        BytesCount = BytesCount + BytesRead;

      } /* while ()

      /* end counter */
      gettimeofday(&End, NULL);
      
      /* update time */
      At = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);
      
      /* show time */
      printf("%f\n", At);

      /* rewind the file again */
      lseek(FileHandle, 0, SEEK_SET);

      /* de-allocate swap buffer */
      free(SwapBuffer);
 
   } /* for(i) */

   /* de-allocate buffers */
   for (i = 0; i < BUFF_COUNT; i++)
   {

     free(Buffer[i]);
 
   } /* for () */

   /* close file */
   close(FileHandle);

   /* return default value */
   return(0);
    
} /* main () */

