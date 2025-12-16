#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int i;
  for(i = 0; i < 10; i++){
    if(fork() == 0){
      int j;
      for(j = 0; j < 5; j++)
        testlock();
      exit();
    }
  }

  while(wait() > 0);
  exit();
}
