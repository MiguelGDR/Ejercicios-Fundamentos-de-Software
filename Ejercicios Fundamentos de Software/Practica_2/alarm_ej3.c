#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void m(int signo)
{
    raise(SIGINT);
}

void man_alarm(int signo)
{
    signal(SIGINT, m);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Uso: %s <segundos de espera>.\n", argv[0]);
        exit(1);
    }

    if(signal(SIGINT,SIG_IGN) == SIG_ERR)
    {
        perror("signal SIGINT");
        exit(1);
    }
    alarm(atoi(argv[1]));

    if(signal(SIGALRM, man_alarm) == SIG_ERR)
    {
        perror("signal SIGALRM");
        exit(1);
    }

    while(1)
    {
        pause();
    }

    return 0;
}