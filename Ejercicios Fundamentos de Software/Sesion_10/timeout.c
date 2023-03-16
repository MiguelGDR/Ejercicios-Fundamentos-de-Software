#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t pid_hijo;

void alarma(int signo)
{
    kill(pid_hijo, SIGKILL);
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Uso: %s <segundos> <comando>.\n", argv[0]);
        exit(1);
    }

    // Clono
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if(pid == 0)
    {
        // Hijo
        execl("/usr/bin/bash", "bash", "-c", argv[2], NULL);
        perror("execl");
        exit(1);
    }
    else
    {
        // Padre

        pid = pid_hijo; // Guardo el valor del pid del hijo para usarlo en el manejador

        // Preparo manejador
        if(signal(SIGALRM, alarma) == SIG_ERR)
        {
            perror("signal SIGALRM");
            exit(1);
        }

        // Ajustes de temporizador
        struct itimerval timer;
        struct timeval valor;

        valor.tv_sec = atoi(argv[1]);
        valor.tv_usec = 0;

        timer.it_value = valor;
        timer.it_interval = valor; 

        // Inicio el temporizador
        if(setitimer(ITIMER_REAL, &timer, NULL) < 0)
        {
            perror("timer");
            exit(1);
        }

        wait(0);
    }

    

    return 0;
}