Author: Jin Ruan

Email: jruan3@wisc.edu

## Goal
#### Project 4a: Scalable Web Server
To add one key piece of functionality to the basic web server: make it multi-threaded.

#### Project 4b: xv6 Threads
To add real kernel threads to xv6. Specifically, first define a new system call to create a kernel thread, called clone() , as well as one to wait for a thread called join(). Then, use clone() to build a little thread library, with a thread_create() , lock_acquire() , and lock_release().

## My work

#### Linux: Scalable Web Server

Most of the codes are added in server.c. Main thread works as the producer, 
which puts the connection fd into the buffer pool. Worker threads act as 
the consumers, which takes the connection fd from the buffer pool and 
handles it. The producer-consumer relationship is built.

#### xv6: Kernel Threads

System calls clone() and join() are written based on fork() and wait(). 
growproc(), exit() and wait() are also modified in order to be compatible with 
the thread feature. The function calls of the thread library sit in ulib.c. 
Besides, forktest.c is removed from the user program in order to fix malloc 
reference problem.
