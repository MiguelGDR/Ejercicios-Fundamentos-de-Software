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
        char msg[T];
        sprintf(msg, "Soy el hijo con PID: %d, y mi padre ha creado %d hijos.\n", getpid(), i);
        for(int i = 0; i < 10; i++)
        {
            write(1, msg, strlen(msg));
        }

        exit(0);
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