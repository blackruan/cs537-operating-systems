#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int *p = (int *) ((640 * 1024) - 4096 - 1);
    printf(1, "%d\n", *p);
    exit();
}
