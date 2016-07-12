#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int i = 0;
    int j = 1;
    char c = 'h';
    printf(1, "%d\n", i);
    printf(1, "%d\n", &i);
    printf(1, "%d\n", j);
    printf(1, "%d\n", &j);
    printf(1, "%c\n", c);
    printf(1, "%d\n", &c);
    exit();
}
