#include <stdio.h>
#include<unistd.h>
#include <sys/wait.h>

int main(){
    int sleep_times[3] = {10,20,30};
    pid_t child_pids[3];

    for(int i = 0; i < 3; i++){
        pid_t pid = fork();

        if (pid < 0){
            perror("fork failed");
            return 1;
        } else if (pid == 0){
            //child process
            printf("Chils %d: pid = %d, sleeping for %d seconds\n", i, getpid(), sleep_times[i]);
            fflush(stdout);
            sleep(sleep_times[i]);
            printf("Child %d: pid = %d, woke up after %d seconds\n",
                   i, getpid(), sleep_times[i]);
            return 0;
        } else {
            child_pids[i] = pid;
        }
    }
    //parent waits for all 3 childern
    printf("Parent: pid = %d, waiting for all children\n",getpid());
    fflush(stdout);

    for (int i = 0; i< 3; i++){
        int status;
        pid_t exited_pid = waitpid(child_pids[i], &status, 0);
        if(exited_pid == child_pids[i] && WIFEXITED(status)){
            printf("Parent: child %d (pid %d exited with status %d\n)", i , exited_pid, WEXITSTATUS(status));
        }
    }
    printf("Parent: all childern have exited\n");
    return 0;
}
