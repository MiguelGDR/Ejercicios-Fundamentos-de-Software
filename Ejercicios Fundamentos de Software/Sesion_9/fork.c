#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define T 256

pid_t pid1, pid2, pid_padre;

void sigusr1_hijo2(int signo)
{
    char msg[T];
    sprintf(msg,"Soy proceso hijo2 con PID=%d: ya estoy despierto\n", getpid());
    write(1,msg,strlen(msg));
    signal(SIGUSR1, sigusr1_hijo2);
}

void sigusr1_padre(int signo)
{
    char msg[T];
    sprintf(msg,"Soy proceso padre con PID=%d: ya voy!\n", getpid());
    write(1,msg,strlen(msg));

    kill(pid2, SIGUSR1);
    signal(SIGUSR1, sigusr1_padre);
}

void alarma(int signo)
{
    char msg[T];
    sprintf(msg,"Soy proceso hijo1 con PID=%d: papá, despierta a mi hermano\n",getpid());
    write(1,msg,strlen(msg));

    kill(pid_padre, SIGUSR1);
    signal(SIGALRM, alarma);
}

int main()
{
    pid_padre = getpid();

    pid1 = fork();

    if(pid1 < 0)
    {
        perror("fork");
        exit(1);
    }else if(pid1 == 0)
    {
        // Hijo 1
        if(signal(SIGALRM, alarma) == SIG_ERR)
        {
            perror("signal ALARM");
            exit(1);
        }
        
        struct itimerval timer;
        struct timeval valor;

        valor.tv_sec = 2;
        valor.tv_usec = 0;

        timer.it_value = valor;
        timer.it_interval = valor;

        if(setitimer(ITIMER_REAL, &timer, NULL) < 0)
        {
            perror("timer");
            exit(1);
        }

        while(1)
        {
            pause();
        }

        exit(0);
    }else
    {
        // Padre
        pid2 = fork();

        if(pid2 < 0)
        {
            perror("fork");
            exit(1);
        }
        else if(pid2 == 0)
        {
            // Hijo 2
            if(signal(SIGUSR1, sigusr1_hijo2) == SIG_ERR)
            {
                perror("signal SIGUSR1 hijo2");
                exit(1);
            }
            
            while(1)
            {
                pause();
            }
            exit(0);
        }
        else
        {
            // Padre
            if(signal(SIGUSR1, sigusr1_padre) == SIG_ERR)
            {
                perror("signal SIGUSR1 padre");
                exit(1);
            }

            while(1)
            {
                pause();
            }

           wait(0);
        }
        wait(0);
    }

}