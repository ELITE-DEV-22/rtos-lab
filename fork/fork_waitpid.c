#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // child process
        printf("Child: %d, Parent: %d\n", getpid(), getppid());
    }
    else
    {
        // parent process
        printf("Parent: %d, Child: %d", getpid(), pid);

        int status;
        pid_t exited_pid = waitpid(pid, &status, 0); // wait specifically for this childs pid

        id(exited_pid == pid)
        {
            if (WIFEXITED(status))
                print("Parent: child %d exited with status %d\n", exited_pid, WEXITSTATUS(status)) else print("Parent: child %d did not exit normally\n", exited_pid);
        }
    }
    return 0;
}