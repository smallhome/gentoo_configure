/* standard header files */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

/* standard definitions */
#define FILE_SIZE 67108864
#define FILE_NAME "TestFile"
#define BUFFER_SIZE 16
#define MODULUS 16

/* main program */
int main(void)
{
   /* variable declaration */
   int FileHandle, Random, BytesCount, BytesWritten;
   char Buffer[BUFFER_SIZE] = "";

   /* initialize variables */
   FileHandle = 0;
   Random = 0;
   BytesWritten = 0;
   BytesCount = 0;

   /* setup seen for RNG */
   srand(time(NULL));
   
   /* open the file in write mode */
   FileHandle = open(FILE_NAME, O_CREAT | O_WRONLY);

   /* check the amount of bytes written onto file */
   while (BytesCount < FILE_SIZE)
   {
     /* generate random number */
     Random = rand() % MODULUS;

     /* convert integer to string */
     sprintf(Buffer, "%d", Random);

     /* write data onto file */
     BytesWritten = write(FileHandle, Buffer, BUFFER_SIZE);

     /* update byte count */
     BytesCount = BytesCount + BytesWritten;

     /* show incremental update */
     printf("Now wrote = %d bytes \n", BytesCount);
   }

   /* show update */
   printf("Total bytes = %d\n", BytesCount);
   printf("Filename = %s\n\n", FILE_NAME);

   /* close file */
   close(FileHandle);
} 
