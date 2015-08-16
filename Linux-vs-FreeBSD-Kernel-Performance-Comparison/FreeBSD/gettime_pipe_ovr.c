/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* standard definitions */
#define PIPE_THRESHOLD 1000
#define TF 1000000

/* forward declaration */
float AverageTime(long T[], int Count);

/* main program */
int main(void)
{
  /* variable declaration */
  struct timeval Start, End;
  int i, PipeDesc[2];
  char Buffer[4]="BUFF";
  long T[PIPE_THRESHOLD];
  float At;

  /* create pipe */
  pipe(PipeDesc);
  
  /* main loop */
  for (i = 0; i < PIPE_THRESHOLD; i++)
  {
     /* start counter */
     gettimeofday(&Start, NULL);

     /* write to pipe and read from it */
     write(PipeDesc[1], Buffer, 4);

     read(PipeDesc[0], Buffer, 4);
  
     /* end counter */
     gettimeofday(&End, NULL);

     /* update time */
     T[i] = (End.tv_sec*TF+End.tv_usec) - (Start.tv_sec*TF+Start.tv_usec);
   }

   /* calulate average */
   At = AverageTime(T, PIPE_THRESHOLD);
   printf("%f\n", At);

   /* return default value */
   return (0);

} /* main () */

/* calculate average time */
float AverageTime(long T[], int Count)
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

