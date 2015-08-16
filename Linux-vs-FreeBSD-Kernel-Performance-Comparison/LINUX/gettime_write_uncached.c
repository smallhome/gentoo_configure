/* standard header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* standard definitions */
#define TF 1000000

/* main program */
int main(void)
{
  /* variable declaration */
  int i, BytesWrite, Fd;
  char Buffer[1] = "B";
  struct timeval Start, End;
  long T;
  float Overhead = 0.21;
  
  /* open /dev/null */
  Fd = open("/dev/null", O_WRONLY);

  /* start counter */
  gettimeofday(&Start, NULL);

  /* write 1 byte to /dev/null */
  BytesWrite = write(Fd, Buffer, 1);
    
  /* end counter */
  gettimeofday(&End, NULL);
    
  T = (End.tv_sec*TF+End.tv_usec)-(Start.tv_sec*TF+Start.tv_usec);
   
  printf("%f\n", (T-Overhead));
   
  return(0);
    
} /* main () */

