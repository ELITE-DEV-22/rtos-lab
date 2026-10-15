fork() / exec() example programs
=================================

All programs compile with gcc on Linux. Build each one, then run it
from a terminal.

1. fork_print.c
   Basic fork() — parent and child print different text.

     gcc -o fork_print fork_print.c
     ./fork_print

2. fork_waitpid.c
   Parent waits specifically for its child's pid (waitpid instead of wait).

     gcc -o fork_waitpid fork_waitpid.c
     ./fork_waitpid

3. fork_sleep.c
   Same as above, but the child sleeps for 20 seconds before exiting.
   The parent blocks in waitpid() for the full 20 seconds.

     gcc -o fork_sleep fork_sleep.c
     ./fork_sleep

4. fork_three_children.c
   Parent forks 3 children (sleeping 10s, 20s and 30s respectively)
   and waits for all of them in order.

     gcc -o fork_three_children fork_three_children.c
     ./fork_three_children

5. rec_fork.c
   A 4-level chain: original parent -> child1 -> child2 -> child3.
   Each process forks the next one and waits for it. Every process
   also sleeps 10s after its own child exits, so total runtime is
   about 30 seconds.

     gcc -o rec_fork rec_fork.c
     ./rec_fork

   While it's running, inspect the ancestry chain in another terminal:

     pstree -p $(pgrep -f rec_fork | head -1)

6. fork_exec.c + exec_prog.c
   fork_exec forks a child, and the child immediately execs into
   exec_prog, which counts from 1 to 10 (one number per second).

     gcc -o exec_prog exec_prog.c
     gcc -o fork_exec fork_exec.c
     ./fork_exec

   While it's running (~10 seconds), check the parent/child relationship
   and confirm the pid stays the same across the exec():

     ps -o pid,ppid,cmd -C exec_prog
     pstree -p $(pgrep fork_exec)
     cat /proc/$(pgrep exec_prog)/status | grep -E 'Pid|PPid'

Notes
-----
- fork_exec.c and exec_prog.c must be built and run from the same
  directory, since fork_exec.c execs "./exec_prog" as a relative path.
- All other programs are standalone and can be built/run independently.