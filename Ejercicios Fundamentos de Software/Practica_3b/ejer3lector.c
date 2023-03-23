#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define T 32

int main()
{
    // CODIGO PROGRAMA LECTOR
    // Leo de la fifo
    int fd;
    if((fd = open("f1", O_RDONLY)) < 0)
    {
        perror("open f1");
        exit(1);
    }

    int rd, wr;
    char buffer[T];
    rd = read(fd, buffer, T);

    while(rd > 0)
    {
        if((wr = write(1, buffer, rd)) < 0) // Escribo por pantalla
        {
            perror("write lector pantalla");
            exit(1);
        }

        rd = read(fd, buffer, T);         // Leo de nuevo
    }
    if(rd < 0)
    {
        perror("read");
        exit(1);
    }
    if(rd == 0) // Se cerró la fifo
    {
        if(close(fd) < 0)     // Cierro el extremo de lectura 
        {
            perror("close lectura lector");
            exit(1);
        }
    }
    
    return 0;
}