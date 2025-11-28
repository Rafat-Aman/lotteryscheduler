// user/lotterytest.c
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc < 3){
    printf(1, "Usage: lotterytest <tickets> <iterations>\n");
    exit();
  }

  int tickets = atoi(argv[1]);
  int loops   = atoi(argv[2]);

  set_base_tickets(tickets);

  // Do some work and make sure we cross a real timer tick each round.
  for(int i = 0; i < loops; i++){
    uint start = uptime();
    // Stay busy until at least one timer tick occurs while running.
    while(uptime() == start){
      for(volatile int j = 0; j < 1000000; j++) {
        // busy-wait to burn CPU
      }
    }
    sleep(1);  // yield on a real tick
  }

  printf(1, "Process %d finished\n", getpid());
  exit();
}
