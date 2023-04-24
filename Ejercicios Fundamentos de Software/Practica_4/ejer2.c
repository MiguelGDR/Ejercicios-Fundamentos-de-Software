#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define T 32

// Función que calcula el max de entre dos enteros (descriptores)
int max_fd(int fd1, int fd2)
{
    if(fd1 > fd2)
    {
        return fd1;
    }
    return fd2;
}

void copia_fdset(fd_set *copia, fd_set *origen, int maxfdmasuno)
{
    for(int i = 0; i < maxfdmasuno; i++)
    {
        if(FD_ISSET(i, origen))
        {
            FD_SET(i, copia);
        }
    }
}

int main()
{
    // Abro la fifo 
    int fd_fifo;

    if((fd_fifo = open("fsc_chat", O_RDONLY)) < 0)
    {
        perror("open fifo");
        exit(1);
    }

    fd_set rfd, copia_rfd;

    FD_ZERO(&rfd);
    FD_SET(0, &rfd);
    FD_SET(fd_fifo, &rfd);

    // Timeout
    struct timeval t, t_copia;
    t.tv_sec = 3;
    t.tv_usec = 0;

    while(1)
    {
        copia_fdset(&copia_rfd, &rfd, max_fd(0, fd_fifo) + 1);
        t_copia = t;

        int rc = select((max_fd(0, fd_fifo) + 1), &copia_rfd, NULL, NULL, &t_copia);

        if(rc == 0)
        {
            // Saltó el temporizador de 3 segundos
            printf(".\n");
        }
        else if(rc < 0)
        {
            perror("select");
            close(fd_fifo);
            exit(1);
        }

        // Procedo a leer de los descriptores activos
        char buffer[T];

        if(FD_ISSET(0, &copia_rfd))
        {
            int rd, wr; 
            if((rd = read(0, buffer, T)) < 0)
            {
                perror("read fd = 0");
                exit(1);
            }
            if((wr = write(1, buffer, rd)) < 0)
            {
                perror("write");
                exit(1);
            }
        }
        if(FD_ISSET(fd_fifo, &copia_rfd))
        {
            int rd, wr;
            if((rd = read(fd_fifo, buffer, T)) < 0)
            {
                perror("read fd = fd_fifo");
                exit(1);   
            }
            if((wr = write(1, buffer, rd)) < 0)
            {
                perror("write");
                exit(1);
            }
        }
    }

    return 0;
}