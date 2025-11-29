#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf(1, "Usage: ticketstat <pid>\n");
    exit();
  }

  int pid = atoi(argv[1]);
  struct ticketinfo info;

  if(update_ticket_status(pid, &info) < 0){
    printf(1, "Error: invalid PID\n");
    exit();
  }

  printf(1, "PID %d:\n", pid);
  printf(1, "  base = %d\n", info.base_tickets);
  printf(1, "  acc  = %d\n", info.accumulated_tickets);
  printf(1, "  exch = %d\n", info.exchanged_tickets);
  printf(1, "  ticks= %d\n", info.ticks);

  exit();
}
