#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    char *p = (char*) (440 * 1024);
    write(1, p, 1);
    exit();
}
