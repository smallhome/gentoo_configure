/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

/* standard definitions */
#define TF 1000000

/* forward declaration */
void SignalHandler(int Arg);

/* main program */
int main(void)
{
  /* variable declaration */
  int i, ProcessID;
  struct timeval Start, End;
  long T;
  float Overhead = 0.21;

  /* setup signal handler */
  signal(SIGUSR1, SignalHandler);
  
  /* start counter */
  gettimeofday(&Start, NULL);

  /* send self SIGUSR1 signal */
  kill(getpid(), SIGUSR1);
    
  /* end counter */
  gettimeofday(&End, NULL);
    
  T = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);
   
  printf("%f\n", (T-Overhead));
   
  return(0);
    
} /* main () */

/* signal handler */
void SignalHandler(int Arg)
{

}

