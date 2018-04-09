#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern int sched_trace_enabled;
int sys_enable_sched_trace(void)
{
  if (argint(0, &sched_trace_enabled) < 0)
  {
    cprintf("enable_sched_trace() failed!\n");
  }

  return 0;
}
int sys_shutdown(void)
{
  /* Either of the following will work. Does not harm to put them together. */
  outw(0xB004, 0x0|0x2000); // working for old qemu
  outw(0x604, 0x0|0x2000); // working for newer qemu

  return 0;
}


int
sys_setrunningticks(void)
{
 int time_allotment;
 if(argint(0,&time_allotment) < 0)
 {
   return 1;
 }
 return setrunningticks(time_allotment);
}

// Proj2: setwaitingticks system call implemented
// extern int waitingticks;
// extern int setwaitingticks(void);
int
sys_setwaitingticks(void)
{
 int waiting_thres;
  if(argint(0,&waiting_thres) < 0)
   {
     return 1;
   }
   return setwaitingticks(waiting_thres);

}

// Proj2: setpriority system call implemented
// extern int pid, priority;
// extern void setpriority(void);
int
sys_setpriority(void)
{
 int pid,priority;
   if(argint(0,&pid) < 0)
   {
     return 1;
   }
   if(argint(1, &priority)< 0)
   {
     return 1;
   }

 return setpriority(pid,priority); 
}




