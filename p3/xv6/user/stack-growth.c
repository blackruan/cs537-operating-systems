#include "types.h"
#include "user.h"

#define LENGTH 500

void
stackGrowth(int iter)
{
  if(iter == 0)
    return;
  printf(1, "iteration left: %d\n", iter);
  int local[LENGTH];
  local[0] = 10;
  local[LENGTH - 1] = 14999;
  printf(1, "first element:%d\n", local[0]);
  printf(1, "array starts:%d\n", &local[0]);
  printf(1, "last element:%d\n", local[LENGTH - 1]);
  printf(1, "array ends:%d\n", &local[LENGTH - 1]);
  stackGrowth(iter - 1);
}

int
main(int argc, char *argv[])
{
  stackGrowth(10);
  exit();
}
