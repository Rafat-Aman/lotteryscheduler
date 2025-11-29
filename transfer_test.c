// transfer_test.c
#include "types.h"
#include "stat.h"
#include "user.h"

int
busyloop(int loops)
{
  volatile int x = 0;
  for(int i = 0; i < loops * 100000; i++) {
    x++;
    if ((i & 0x1FFF) == 0) sleep(1);
  }
  return x;
}

int
main(int argc, char *argv[])
{
  int pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }

  if(pid == 0){
    // child: set a low base so donor (parent) has more
    set_base_tickets(10);
    busyloop(50);
    printf(1, "child %d done\n", getpid());
    sleep(200);  // stay alive so ticketstat can inspect
    exit();
  } else {
    // parent: ensure it has larger base, then donate to child
    set_base_tickets(100);
    // donate 50 tickets to child
    int donated = transfer_tickets(pid, 50);
    printf(1, "parent donated %d tickets to %d\n", donated, pid);

    // give shell time to query while child is alive
    sleep(100);
    // exit without wait so child is adopted by init
    exit();
  }
}
