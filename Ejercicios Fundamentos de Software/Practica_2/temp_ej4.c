#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void manejador(int signo)
{   
    write(1, ".\n", 2);
    signal(SIGALRM, manejador);
}

int main()
{
    struct itimerval timer;
    struct timeval valor;

    valor.tv_sec = 3;
    valor.tv_usec = 300000;

    timer.it_value = valor;
    timer.it_interval = valor;

    if(signal(SIGINT, SIG_IGN) == SIG_ERR)
    {
        perror("signal SIGINT ignore");
        exit(1);
    }

    if(signal(SIGALRM, manejador) == SIG_ERR)
    {
        perror("signal SIGALRM");
        exit(1);
    }
    if(setitimer(ITIMER_REAL, &timer, NULL) < 0)
    {
        perror("timer");
        exit(1);
    }

    while(1)
    {
        pause();
    }

    return 0;
}