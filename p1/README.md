Author: Jin Ruan

Email: jruan3@wisc.edu

## Goal
#### Project 1a: Sorting within a Range
To build a sorting program called rangesort that takes in one of the generated files and sorts it based on the 4-byte key (the remainder of the record should of course be kept with the same key). Keys which fall outside of lowvalue and highvalue should be discarded (and not written to the output file). The output is written to the specified output file.

#### Project 1b: xv6 Intro
To add two system calls to xv6. The system calls are:

1. getsyscallnumtotal() returns the total number of system calls that have been issued.
2. getsyscallnumsuccess() returns the number of system calls that have completed successfully (i.e., did not have a return value of -1).

## My work

#### Linux: rangesort

Discard the unqualified records when read-in, i.e. before sorting. Use strtoul() to convert the strings into numbers and do error handling. Store the lowValue and highValue into lowValue_ul and highValue_ul first and perform validation check. Assigned to lowValue_u and highValue_u if passed, which are used for discarding.

#### xv6: add two system calls

Make changes in the following files:
user/user.h, user/usys.S, kernel/sysfunc.h, kernel/proc.h, kernel/syscall.c, kernel/sysproc.c, include/syscall.h

The crucial part is in syscall() function of kernel/syscall.c. We increment the two global counters in it.
