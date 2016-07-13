Author: Jin Ruan

Email: jruan3@wisc.edu

## Goal
#### Project 3: The Null Pointer and Other Tricks

To change xv6 to support a few features virtually every modern OS does. The first is causing an exception to occur when your program dereferences a null pointer; the second is rearranging the address space so as to place the stack at the high end.

## My work

#### Part A: Null-pointer Dereference

For this part, I built the user address space from PGSIZE in exec.c and 
modified the copyuvm() in vm.c to make it look up from PGSIZE. Also, in 
makefile the address of section start begins from 0x1000 instead of 0x0.

#### Part B: Stack Rearrangement

1. I created a new stack starts from USERTOP - PGSIZE to USERTOP in 
allocuvm(). New variable sz_stack is added in the proc structure to keep 
track of the top of the stack while sz is still used to store the size of 
code and heap.
2. In copyuvm(), I modified it to also copy the stack while this function 
is used.
3. When there is a page fault happened (trapno == 14) and the esp is out of 
the stack, we need to grow stack. And I write a function growstack() in proc.c 
to make it.
4. Furthermore, in growproc() and growstack(), codes are added to keep one 
dummy page between heap and stack.
5. Last but not least, in syscall.c, all places related to proc->sz are 
needed to modify. I modified the boundary carefully in fetchint(), fetchstr(), 
argptr().
