#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid < 0){
        perror("fork failed");
        return 1;
    } else if (pid == 0){
        //child process
        printf("child: my pid = %d, parent pid = %d\n", getptd(), getppid());
    } else{
        //parent process
        printf("Parent: my pid = %d, child pid = %d\n", getpid(), pid);
        wait(NULL); //wait for child process to finish
        printf("Parent: child has exited\n");
    }
    return 0;
}