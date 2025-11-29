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

int 
sys_hWorld(void)
{
    cprintf("Hello World from syscall!\n");
    return 0;
}
//rafat
int
sys_set_base_tickets(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;

  if (n < 1) 
    n = 1;   // minimum 1 ticket

  struct proc *p = myproc();
  p->base_tickets = n;

  return 0;
}
//step04
int
sys_update_ticket_status(void)
{
    int pid;
    struct ticketinfo *uinfo;
    struct proc *p;

    // Read arguments: pid and user pointer
    if (argint(0, &pid) < 0 || argptr(1, (void*)&uinfo, sizeof(*uinfo)) < 0)
        return -1;

    acquire(&ptable.lock);

    // Find process
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->pid == pid) {
            // Copy kernel values into user struct
            uinfo->base_tickets = p->base_tickets;
            uinfo->accumulated_tickets = p->accumulated_tickets;
            uinfo->exchanged_tickets = p->exchanged_tickets;
            uinfo->ticks = p->ticks;

            release(&ptable.lock);
            return 0;
        }
    }

    release(&ptable.lock);
    return -1;  // pid not found
}
