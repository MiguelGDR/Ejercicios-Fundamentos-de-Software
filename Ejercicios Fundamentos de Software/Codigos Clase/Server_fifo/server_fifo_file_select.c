#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define T 32

void copia_fdset(fd_set *dst, fd_set *src, int maxfdmasuno)
{
    FD_ZERO(dst);

    for(int i = 0; i < maxfdmasuno; i++)
    {
        if(FD_ISSET(i, src))
        {
            FD_SET(i, dst);
        }
    }
}

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

    // Estructuras de datos para el select
    fd_set cjt_original, cjt_modificado;
    FD_ZERO(&cjt_original);
    FD_ZERO(&cjt_modificado);   // Opcional

    // Incluir fifo1 y fifo2 en el cjt_original
    FD_SET(fifo1, &cjt_original);
    FD_SET(fifo2, &cjt_original);

    // Calculamos el máximo + 1
    int maxfd;
    if(fifo1 > fifo2)
    {
        maxfd = fifo1;
    }
    else
    {
        maxfd = fifo2;
    }

    // Timeout
    struct timeval t, t_copia;
    t.tv_sec = 15;
    t.tv_usec = 0;

    int l1, l2, conectados = 2;
    char b[T];
    while(1)
    {
        printf("Esperando informacion de los procesos...\n");

        // Hacemos una copia de los argumentos del select
        copia_fdset(&cjt_modificado, &cjt_original, maxfd + 1);
        t_copia = t;
        int r = select(maxfd + 1, &cjt_modificado, NULL, NULL, &t_copia);
        if(r < 0)
        {
            perror("select");
            exit(1);
        }
        if(r == 0)
        {
            // Temporizador expirado
            printf("Temporizador expirado.\n");
        }
        
        // Comprobamos qé descriptores de fichero están listos para lectura
        if(FD_ISSET(fifo1, &cjt_modificado))
        {
            // Fifo1 esta listo para lectura, ahora puedo leer sin bloquearme
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
                FD_CLR(fifo1, &cjt_original);   // No vuelvo a leer de él

                conectados--;
                if(conectados == 0)
                {
                    break;
                }
            }
        }

        if(FD_ISSET(fifo2, &cjt_modificado))
        {
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
                FD_CLR(fifo2, &cjt_original);

                conectados--;
                if(conectados == 0)
                {
                    break;
                }
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