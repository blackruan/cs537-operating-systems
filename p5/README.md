Author: Jin Ruan

Email: jruan3@wisc.edu

## Goal
#### Project 5: File System Integrity
To change the existing xv6 file system to add protection from data corruption.

Specifically, there are three things. First, modify the code to allow the user to create a new type of file that keeps a checksum for every block it points to. Checksums are used by modern storage systems in order to detect silent corruption.

Second, change the file system to handle reads and writes differently for files with checksums. Specifically, when writing out such a file, create a checksum for every block of the file; when reading such a file, check and make sure the block still matches the stored checksum, returning an error code (-1) if it doesn't. In this way, your file system will be able to detect corruption!

Third, for information purposes, you will also modify the stat() system call to dump some information about the file. Thus, you should write a little program that, given a file name, not only prints out the file's size, etc., but also some information about the file's checksums.

## My work

#### xv6: File System Integrity
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
