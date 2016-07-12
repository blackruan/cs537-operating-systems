#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    char *p = 0;
    write(1, p, 1);
    exit();
}
