#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define T 32

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Uso: %s <fifo1> <fifo2>.\n", argv[0]);
        exit(1);
    }

    // Descriptores de fichero
    int fifo1, fifo2, fd;

    // Apertura del fichero
    if((fd = open("logs.txt", O_WRONLY | O_CREAT | O_APPEND, 0640)) < 0)
    {
        perror("open logs.txt");
        exit(1);
    }

    // Apertura de las FIFOS
    if((fifo1 = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open fifo1");
        exit(1);
    }
    if((fifo2 = open(argv[2], O_RDONLY)) < 0)
    {
        perror("open fifo2");
        exit(1);
    }

    int l1, l2, conectados = 2;
    char b[T];
    while(1)
    {
        printf("Esperando informacion de los procesos...\n");
        l1 = read(fifo1, b, T);
        if(l1 < 0)
        {
            perror("read fifo1");
            exit(1);
        }
        else if(l1 > 0)
        {
            // Compongo el mensaje a escribir
            char line[T] = "Proceso A: ";
            strcat(line, b);
            int length = strlen(line);
            
            // Escribo el mensaje en disco
            if(write(fd, line, length) != length)
            {
                perror("write fifo1");
                exit(1);
            }
        }
        else
        {
            // l1 == 0
            conectados--;
            if(conectados == 0)
            {
                break;
            }
        }

        l2 = read(fifo2, b, T);
        if(l2 < 0)
        {
            perror("read fifo2");
            exit(1);
        }
        else if(l2 > 0)
        {
            // Compongo el mensaje a escribir
            char line[T] = "Proceso B: ";
            strcat(line, b);
            int length = strlen(line);
            
            // Escribo el mensaje en disco
            if(write(fd, line, length) != length)
            {
                perror("write fifo2");
                exit(1);
            }
        }
        else
        {
            // l2 == 0
            conectados--;
            if(conectados == 0)
            {
                break;
            }
        }
    }

    if(close(fifo1) < 0)
    {
        perror("close fifo1");
        exit(1);
    }
    if(close(fifo2) < 0)
    {
        perror("close fifo2");
        exit(1);
    }
    if(close(fd) < 0)
    {
        perror("close logs.txt");
        exit(1);
    }

    return 0;
}