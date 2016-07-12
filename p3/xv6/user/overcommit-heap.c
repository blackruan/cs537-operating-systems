#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int j[10000];
  j[0] = 100;
  //printf(1, "%d\n", &j[9999]);
  int k;
  for(k = 0; k < (640 * 1024); k++) {
    int *i = (int *)malloc(sizeof(int));
    *i = 100;
    //printf(1, "%d\n", i);
  }
  exit();
}
