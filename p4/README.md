Author: Jin Ruan
Email: jruan3@wisc.edu
CS login: jin

Linux: Scalable Web Server
Most of the codes are added in server.c. Main thread works as the producer, 
which puts the connection fd into the buffer pool. Worker threads act as 
the consumers, which takes the connection fd from the buffer pool and 
handles it. The producer-consumer relationship is built by referring to the 
relevant codes in the lecture slides.

No bugs found.

xv6: Kernel Threads
System calls clone() and join() are written based on fork() and wait(). 
growproc(), exit() and wait() are also modified in order to be compatible with 
the thread feature. The function calls of the thread library sit in ulib.c. 
Besides, forktest.c is removed from the user program in order to fix malloc 
reference problem.

No bugs found.
