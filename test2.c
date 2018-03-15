/* Test 2 -- test queue1, the priority queue
In this test, we set waiting ticks to be a large number
--- process in the priority queue cannot move back to the round robin queue

In the beginning, C is forced to stay in the round robin queue (using setpriority)
After a while, C is set to change its priority, and will move to priority queue.
During this time, process A and B should accumulate some waiting ticks.

So after C changes its priority, we except that process A and B should execute for while 
before process C starts executing (as C has the lowest priority).

The results after C changes its priority should be:
......ABABABABABAB.....ABCABCABC....

or something like
......AABBAABBAABB.....ABBCBBABBCBB......
(depends on how you deal with two processes with the same priority value).
*/

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void loop()
{
  int i;
  int j=0;
  for(i=0;i<1000000;i++)
    j=j+1;
}
void
forktest(void)
{
  int pid1;
  int ret;
  int fds1[2];

  ret = setrunningticks(2);
  if (ret < 0)
  {
    printf(1, "cannot set running ticks\n");
    exit();
  }
  ret = setwaitingticks(1000000);
    if (ret < 0)
  {
    printf(1, "cannot set waiting ticks\n");
    exit();
  }

  ret = pipe(fds1);
  if ( ret < 0)
  {
    printf(1, "cannot create a pipe\n");
    exit();
  }

  pid1 = fork();
  if(pid1 < 0)
    return;
    
  if(pid1 == 0){
      int i;
      char buf[256];
      // block here
      close(fds1[1]);
      read(fds1[0], buf, 1);
      printf(1, "\n start process A [%d]\n", getpid());

      for (i=0;i<100;i++)
      {
        //printf(1, "Program C[%d] %d\n", getpid(), i);
	loop();
      }
  }
  else
  {
    int pid2;
    int fds2[2];
    ret = pipe(fds2);

    ret = pipe(fds2);
    if (ret < 0)
    {
        printf(1, "cannot create the second pipe\n");
    }

    pid2 = fork();
    if(pid2 < 0)
      return;
    
    if(pid2 == 0){
      int i;
      char buf[256];
      close(fds1[0]);
      close(fds2[1]);
      //block here
      read(fds2[0], buf, 1);
      write(fds1[1], "Done", 5);
      printf(1, "\n start process B [%d]\n", getpid());

      for (i=0;i<100;i++)
      {
	//printf(1, "Program B[%d] %d\n", getpid(), i);
        loop();
      }
    }
    else
    {
      int i;
      close(fds1[0]);
      close(fds1[1]);
      close(fds2[0]);
      write(fds2[1],"Done", 5);
      printf(1, "\n start process C [%d]\n", getpid());


      setpriority(getpid(), 0);
      for (i=0;i<25;i++)
      {
        //printf(1, "Program A[%d] %d\n", getpid(), i);
        loop();
      }
      setpriority(getpid(), 1);
      printf(1, "\n C prority changes\n");

      for (i=0;i<50;i++)
      {
        //printf(1, "Program A[%d] %d\n", getpid(), i);
        loop();
      }

      wait();
      wait();
    }
  }
}

int
main(void)
{
  forktest();
  exit();
}
