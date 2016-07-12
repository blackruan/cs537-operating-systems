Author: Jin Ruan
Email: jruan3@wisc.edu
CS login: jin

xv6: File System Integrity
1) Added new file type T_CHECKED and flag O_CHECKED. Also, checksum field 
is added in struct stat.

2) Modified open and create system calls to handle file type T_CHECKED and 
flag O_CHECKED.

3) In kernel/fs.c, functions readi() and writei() are modified to fit with 
the new checksum feature. Besides, in bmap(), address returned for file 
T_CHECKED was only 3 bytes long.

4) Three helper functions were built as described below.
csget(): get the checksum field of the data block
cscompute(): compute the checksum for the data block
csupdate(): update the checksum field of the data block

5) Function stati() was modified to fit with the checksum feature.

No bugs found.
