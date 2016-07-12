#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf(1, "filestat pathname\n");
    exit();
  }

  int fd;
  struct stat st;

  if((fd = open(argv[1], 0)) < 0) {
    printf(2, "filestat: cannot open %s\n", argv[1]);
    exit();
  }
  if(fstat(fd, &st) < 0) {
    printf(2, "filestat: cannot stat %s\n", argv[1]);
    close(fd);
    exit();
  }

  switch(st.type) {
    case T_DIR:
      printf(1, "Type: %d\nSize: %d\n", st.type, st.size);
      break;
    case T_FILE:
      printf(1, "Type: %d\nSize: %d\n", st.type, st.size);
      break;
    case T_DEV:
      printf(1, "Type: %d\nSize: %d\n", st.type, st.size);
      break;
    case T_CHECKED:
      printf(1, "Type: %d\nSize: %d\nChecksum: %d\n", st.type, st.size, 
          st.checksum);
      break;
  }
  close(fd);
  exit();
}
