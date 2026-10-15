#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    if (pid < 0){
        perror("Process Failed")
    } else if(pid == 0){
        //child process
        printf("Child: %d, Parent: %d\n", getpid(), getppid());
        printf(" Child: sleeping for 20 seconds");
        fflush(stdout);
        sleep(20);
        printf("Child: woke up, exciting now\n");
    } else {
        //parent process
        printf("Parent: %d, child: %d", getpid(), pid);
        printf("Parent waiting for child to finish \n");
        fflush(stdout);

        int status;
        pid_t exited_pid = waitpid(pid, &status, 0); //blocks here for ~20 sec

        if (ecited_pid == pid){
            if(WIFEXITED(status))
                print("Parent: child %d exited with status %d\n", exited_pid, WEXITSTATUS(status));
            else
                printf("Parent: child %d did not exit normally\n", exited_pid);
        }
    }
    return 0;
}