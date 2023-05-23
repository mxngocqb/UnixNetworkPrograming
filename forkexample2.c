#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void handler(int signum)
{
    if (signum == SIGCHLD)
    {
        int status = 0;
        pid_t pid;
        while ((pid = wait(&status)) > 0)
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            printf("A child process: %d has terminated\n", pid); 
        };
    }
}
int main()
{
    signal(SIGCHLD, handler);

    int x = 1;
    if (fork() == 0)
    {
        printf("Hello World from Child 1: %d\n", x++);
        sleep(1);
        exit(0);
    }else
    {
        printf("Hello World from Parent: %d\n", x--);
        if (fork() == 0)
        {
            printf("Hello World from Child 2: %d\n", x++); 
            sleep(2); 
            exit(0);         
        }else
        {
            if (fork() == 0)
            {
                printf("Hello World from Child 3: %d\n", x++);   
                sleep(3);     
                exit(0);
            }
        }
    }
    getchar();
}