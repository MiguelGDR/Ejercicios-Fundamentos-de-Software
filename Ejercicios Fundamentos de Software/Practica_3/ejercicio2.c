#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define T 60

void wait(int signo)
{
    wait(0);
    signal(SIGCHLD, wait);
}

int main()
{
    pid_t pid = getpid();
    int i = 0;

    while((pid > 0) && (i < 10))
    {
        pid = fork();

        i++;
    }

    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if(pid == 0)
    {
        // Hijo
        // Compruebo si el PID es par o impar 
        if (getpid() % 2 == 0) 
        {
            // Si el PID es par
            execl("/usr/bin/bash", "bash", "-c", "date", NULL);
            perror("execl");
            exit(1);

        } else 
        {
            // Si el PID es impar
            execl("/usr/bin/bash", "bash", "-c", "pwd", NULL);
            perror("execl");
            exit(1);
        }
    }
    else
    {
        // Padre
        // Registro manejador
        if(signal(SIGCHLD, wait) < SIG_ERR)
        {
            perror("signal SIGCHLD");
            exit(1);
        }
    }

    return 0;
}