#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int *i = (int *)malloc(sizeof(int));
  *i = 100;
  printf(1, "%d\n", *i);
  printf(1, "%d\n", i);
  int *j = (int *)malloc(sizeof(int));
  *j = 100;
  printf(1, "%d\n", *j);
  printf(1, "%d\n", j);
  int *k = (int *)malloc(sizeof(int));
  *k = 100;
  printf(1, "%d\n", *k);
  printf(1, "%d\n", k);
  exit();
}
