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

int    // new function added for dump
sys_dump(void)
{
int pid;
void *addr;
void *buffer;
int size;

  if(argint(0, &pid) < 0)
    return -1;
  if(argptr(1, (void*)&addr,sizeof(*addr)) < 0)
    return -1;
  if(argptr(2, (void*)&buffer,sizeof(*buffer)) < 0)
    return -1;
  if(argint(3, &size) < 0)
    return -1;
  return dump(pid, addr, buffer, size);
}

int
sys_thread_create(void)
{
  int function, arg, stack;
  if(argint(0, &function) < 0)
    return -1;
  if(argint(1, &arg) < 0)
    return -1;
  if(argint(2, &stack) < 0)
    return -1;
  return thread_create((void*)function, (void*)arg, (void*)stack);
}

int
sys_thread_join(void)
{
  return thread_join();
}

int
sys_thread_exit(void)
{
  thread_exit();
  return 0;  // not reached
}

int
sys_cond_sleep(void)
{
  void *chan;
  argptr(0, (void*)&chan, sizeof(*chan));
  cond_sleep(chan);
  return 0;
}

int
sys_cond_wake(void)
{
  void *chan;
  argptr(0, (void*)&chan, sizeof(*chan));
  cond_wake(chan);
  return 0;
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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
