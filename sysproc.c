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

int
sys_transfer_tickets(void)
{
  int to_pid, n;
  if(argint(0, &to_pid) < 0) return -1;
  if(argint(1, &n) < 0) return -1;
  if(n <= 0) return -1;

  struct proc *cur = myproc();
  if(cur == 0) return -1;

  acquire(&ptable.lock);

  // find target process
  struct proc *p;
  struct proc *target = 0;
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == to_pid){
      target = p;
      break;
    }
  }
  if(!target){
    release(&ptable.lock);
    return -1;
  }

  // compute available tickets for donor
  int avail = cur->base_tickets + cur->accumulated_tickets - cur->exchanged_tickets;
  if(avail < 1) avail = 1;

  if(n > avail) n = avail; // donate up to avail

  // deduct from accumulated_tickets first
  if(cur->accumulated_tickets >= n){
    cur->accumulated_tickets -= n;
  } else {
    int rem = n - cur->accumulated_tickets;
    cur->accumulated_tickets = 0;
    cur->base_tickets -= rem;
    if(cur->base_tickets < 1) cur->base_tickets = 1;
  }

  // give to target as accumulated_tickets (so it merges later)
  target->accumulated_tickets += n;

  // bookkeeping: donor exchanged more tickets
  cur->exchanged_tickets += n;

  release(&ptable.lock);
  return n;
}
