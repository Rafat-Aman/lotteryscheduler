// tickettest.c
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int t = 10;
  if(argc > 1)
    t = atoi(argv[1]);

  set_base_tickets(t);

  while(1);  // keep running to consume CPU
}
