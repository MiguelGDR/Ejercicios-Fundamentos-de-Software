#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define T 32

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Uso: %s <servers' PID>.\n", argv[0]);
        exit(1);
    }

    int señal;
    
    if((señal = kill(atoi(argv[1]), SIGUSR1)) < 1)
    {
        perror("señal kill");
        exit(1);
    }

    // Abro la fifo para escribir
    int fd_fifo;
    if((fd_fifo = open("fsc_chat", O_WRONLY)) < 0)
    {
        perror("open");
        exit(1);
    }


    while(1)
    {
        char buffer[T];
        int rd, wr;

        if((rd = read(0, buffer, T)) < 0)
        {
            perror("read");
            exit(1);
        }

        if((wr = write(fd_fifo, buffer, rd)) < 0)
        {
            perror("write");
            exit(1);
        }
    }
    
    return 0;
}