#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int handled = 0;

void siginthandler(int signum)
{
    // if (handled) return;
    // handled = 1;
    printf("The pid of this process is %d\n", getpid());
    printf("SIGINT received.\n");
    return;
}

int main()
{

    int pid = fork();

    if (pid)
    {
        printf("The pid of this process is %d\n", getpid());
        signal(SIGINT, siginthandler);
        sleep(10);
        kill(0, SIGKILL);
    }
    else
    {
        printf("The pid of this process is %d\n", getpid());
        signal(SIGINT, siginthandler);
        while (1)
        {
            printf("waiting for sigint\n");
            sleep(1);
        }
    }

    return 0;
}
