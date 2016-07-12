#include "types.h"
#include "user.h"

#define LENGTH 1024 

void
stackGrowth(int iter)
{
  if(iter == 0)
    return;
  printf(1, "iteration left: %d\n", iter);
  int local[LENGTH];
  local[0] = 10;
  stackGrowth(iter - 1);
}

int
main(int argc, char *argv[])
{
  stackGrowth(200);
  exit();
}
