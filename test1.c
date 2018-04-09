/* Test 1 -- test queue0, the round robin queue
In this test: we set running ticks to be a large number
--- process in the round robin queue cannot move to the priority queue
Process A, B, and C should run by turns (intermittently) (in a round robin manner)
You expect some results like
.....CBACBACBACBACBA......
(Notice that, your output should be the pid of process A, B, and C)

Of course, it could be ...ABCABC... or, ...ACBACB..., etc.
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

  ret = setrunningticks(1000000);
  if (ret < 0)
  {
    printf(1, "cannot set running ticks\n");
    exit();
  }
  ret = setwaitingticks(4);
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
      printf(1, "\nstart process A [%d]\n", getpid());

      for (i=0;i<50;i++)
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
      printf(1, "\nstart process B [%d]\n", getpid());

      for (i=0;i<50;i++)
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
      printf(1, "\nstart process C [%d]\n", getpid());

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
