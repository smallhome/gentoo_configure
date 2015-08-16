/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/* standard definitions */
#define SIGNAL_THRESHOLD 1000
#define TF 1000000

/* global variables */
static int PSigCount = 0;
static int CSigCount = 0;

/* forward declaration */
void PSigHandler(int Arg);
void CSigHandler(int Arg);
float AverageTime(long T[], int Count);

/* main program */
void main(int Arg)
{
  /* variable declaration */
  struct timeval Start1, End1, Start2, End2;
  int PID, PPID;
  long T1[SIGNAL_THRESHOLD], T2[SIGNAL_THRESHOLD];
  float At, At1, At2, Overhead_KILL = 3.40;
   
  /* fork a process */
  if ((PID = fork()) != 0) /* parent */
  {
    /* setup child signal handler */
    signal(SIGUSR2, CSigHandler);
    
    /* check the parent signal count */
    while (PSigCount < SIGNAL_THRESHOLD)
    {
      /* start counter */
      gettimeofday(&Start1, NULL);

      /* show signal count */
     // printf("Parent Signal Count = %d\n", PSigCount);
     
      /* send SIGUSR1 signal to child */
      kill(PID, SIGUSR1);

      /* end counter */
      gettimeofday(&End1, NULL);

      /* update time */
      T1[PSigCount] = (End1.tv_sec*TF+End1.tv_usec)-(Start1.tv_sec*TF+Start1.tv_usec);

      /* update parent signal counter */
      PSigCount = PSigCount + 1;

    } /* while () */
  } 
  else /* child */
  {
    /* get process id of parent */
    PPID = getppid();
    
    /* setup parent signal handler */
    signal(SIGUSR1, PSigHandler);

    /* check the child signal count */
    while (CSigCount < SIGNAL_THRESHOLD)
    {
     /* start counter */
     gettimeofday(&Start2, NULL);

     /* show signal count */
    // printf("Child Signal Count = %d\n", CSigCount);
     
     /* sleep for 1 second  to synchronize, parent and child */
     sleep(1);
      
     /* send SIGUSR2 signal to parent */
     kill(PPID, SIGUSR2);
      
     /* end counter */
     gettimeofday(&End2, NULL);
      
     /* update time */
     T2[CSigCount] = (End2.tv_sec*TF+End2.tv_usec)-(Start2.tv_sec*TF+Start2.tv_usec);
       
     /* update child signal counter */
     CSigCount = CSigCount + 1;

    } /* while () */

  } /* if () */
  
  /* check if signal count has reached SIGNAL_THRESHOLD */
  if (PSigCount == SIGNAL_THRESHOLD)
  {
     /* calculate average for parent */
     At1 = AverageTime(T1, SIGNAL_THRESHOLD);
  
     /* calculate average for child */
     At2 = AverageTime(T2, SIGNAL_THRESHOLD);

     /* combine average, with over-head adjustment */
     At = (((At1 + At2)/2) - Overhead_KILL);

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

