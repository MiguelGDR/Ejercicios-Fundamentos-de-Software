#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define T 60

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Uso: %s <comando>.\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if(pid == 0)
    {
        // Hijo
        execl("/usr/bin/bash", "bash", "-c", argv[1], NULL);
        perror("execl");
        exit(1);
    }
    else
    {
        // Padre

        wait(0);
    }
    return 0;
}