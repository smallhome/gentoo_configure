/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

/* standard definitions */
#define PIPE_THRESHOLD 1000
#define TF 1000000

/* global variables */
static int PPipeCount = 0;
static int CPipeCount = 0;

/* forward declaration */
float AverageTime(long T[], int Count);

/* main program */
void main(int Arg)
{
  /* variable declaration */
  struct timeval Start1, End1, Start2, End2;
  int i, PID, PipeDesc1[2], PipeDesc2[2];
  char Buffer1[4], Buffer2[4];
  long T1[PIPE_THRESHOLD], T2[PIPE_THRESHOLD];
  float At, At1, At2, Overhead_PIPE = 0.0;

  Buffer1[4] = '\0';
  Buffer2[4] = '\0';
   
  /* create pipes 1 and 2 */
  pipe(PipeDesc1);
  pipe(PipeDesc2);
   
  /* fork a process */
  if ((PID = fork()) != 0) /* parent */
  {
    /* check the parent  count */
    while (PPipeCount < PIPE_THRESHOLD)
    {
      /* start counter */
      gettimeofday(&Start1, NULL);

      /* send to child on pipe1/1, receive from child on pipe2/0 */
      write(PipeDesc2[1], "BUFF", 4);
      read(PipeDesc1[0], Buffer1, 4);
     // printf("Buffer1 = %s\n", Buffer1);

      /* end counter */
      gettimeofday(&End1, NULL);

      /* update time */
      T1[PPipeCount] = (End1.tv_sec*TF+End1.tv_usec)-(Start1.tv_sec*TF+Start1.tv_usec);

      /* update parent pipe counter */
      PPipeCount = PPipeCount + 1;

    } /* while () */
  } 
  else /* child */
  {
    /* check the child signal count */
    while (CPipeCount < PIPE_THRESHOLD)
    {
     /* start counter */
     gettimeofday(&Start2, NULL);

     /* receive on pipe 1 - 0 and send on pipe 2 - 1 */
     read(PipeDesc2[0], Buffer2, 4);
     write(PipeDesc1[1], "FFUB", 4);
    // printf("Buffer2 = %s\n", Buffer2);
      
     /* end counter */
     gettimeofday(&End2, NULL);
      
     /* update time */
     T2[CPipeCount] = (End2.tv_sec*TF+End2.tv_usec)-(End2.tv_sec*TF+Start2.tv_usec);
       
     /* update child pipe counter */
     CPipeCount = CPipeCount + 1;

    } /* while () */

  } /* if () */
  
  /* check if pipe count has reached PIPE_THRESHOLD */
  if (PPipeCount == PIPE_THRESHOLD)
  {
     /* calculate average for parent */
     At1 = AverageTime(T1, PIPE_THRESHOLD);
  
     /* calculate average for child */
     At2 = AverageTime(T2, PIPE_THRESHOLD);

     /* combine average, with over-head adjustment */
     At = (((At1 + At2)/2) - Overhead_PIPE);

     /* show average */
     printf("%f\n", At);
  } 
    
} /* main () */

/* parent signal handler */
void PSigHandler(int Atg)
{
 // printf("Got Signal from Parent\n");
}

/* child signal handler */
void CSigHandler(int Arg)
{ 
 // printf("Got Signal from Child\n");
}

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

