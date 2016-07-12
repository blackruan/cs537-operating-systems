Author: Jin Ruan
Email: jruan3@wisc.edu
CS login: jin

Linux: my shell
I put most of the parsing, internal command, and fork() into a function called 
runCommand for the convenience of implementing the ! internal command. Circular array 
is used to store the history commands, of which the maximum size is 20. If there 
is redirection, the command stores in the tokens global variable and the redirection 
file stores in the outputs global variable.

No bugs found.

xv6: MLFQ scheduler
To make a MLFQ scheduler, I mainly modified the codes in two functions, alloproc() 
and scheduler(). To store the processes, I built up 4 queues representing four 
levels using singly linked list with head and tail pointer. In alloproc(), I 
enqueued the processes, initialized them and put them at the end of the top level 
priority queue. In scheduler(), I searched from the start of the top level queue 
and as soon as I found a RUNNABLE proc, I chose it to run. After running, if the 
timer ticks of the proc in this level exceeded this time slice of this level, 
dequeued it and added it to the end of the next level ( the lowest level is 
different, needed to add the end of the same level) and then researched RUNNABLE 
proc from the start of the top level queue. Besides, if a UNUSED proc is met in the 
searching, dequeued it.

No bugs found.
