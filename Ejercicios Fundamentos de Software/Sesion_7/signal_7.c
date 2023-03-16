#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define T 16

void m(int signo)
{
    char b[T];
    int cont = 1;
    int wr;

    // Inicio descriptor
    int fd;
    
    // Abro archivo para escribir
    if((fd = open("reg.dat", O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
    {
        perror("open");
        exit(1);
    }

    if(signo == SIGINT)
    {
        sprintf(b,"%d: SIGINT\n", cont);
        if((wr = write(fd, b, sizeof(char) * 10)) < 0)
        {
            perror("write");
            exit(1);
        }

        signal(SIGINT, m);
        cont++;
    }
    else if(signo == SIGUSR1)
    {
        sprintf(b,"%d: SIGUSR1\n", cont);
        if((wr = write(fd, b, sizeof(char) * 11)) < 0)
        {
            perror("write");
            exit(1);
        }

        signal(SIGUSR1, m);
        cont++;
    }
    else if(signo == SIGUSR2)
    {
        sprintf(b,"%d: SIGUSR1\n", cont);
        if((wr = write(fd, b, sizeof(char) * 11)) < 0)
        {
            perror("write");
            exit(1);
        }

        signal(SIGUSR1, m);
        cont++;
    }


    if(close(fd))
    {
        perror("close");
        exit(1);
    }
}

int main(int argc, char * argv[])
{
    // 1. Registro el manejador de señal

    if (signal(SIGUSR1, m) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }
    if(signal(SIGUSR2, m) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }
    if(signal(SIGINT, m) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }

    while(1)
    {
        pause();
    }

    return 0;
}