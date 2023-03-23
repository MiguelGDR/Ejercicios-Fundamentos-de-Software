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
    // CODIGO DEL PROGRAMA ESCRITOR
    // Abro la fifo f1 para escribir
    int fd;
    if((fd = open("f1", O_WRONLY)) < 0)
    {
        perror("open f1");
        exit(1);
    }

    // Abro la fifo f2 para leer la respuesta
    int fd2;
    if((fd2 = open("f2", O_RDONLY)) < 0)
    {
        perror("open f2");
        exit(1);
    }
    // Leo del teclado
    int rd, wr;
    char respuesta[1];
    char y[] = "y", n[] = "n";
    char buffer[T];
    rd = read(0, buffer, T);        

    while(rd > 0)
    {
        if((wr = write(fd, buffer, rd)) < 0)  // Escribo en la fifo
        {
            perror("write escritor fifo");
            exit(1);
        }
        if((rd = read(fd2, respuesta, 1)) < 0)
        {
            perror("read f2");
            exit(1);
        }

        // Compruebo respuesta
        if(strncmp(respuesta, y, strlen(y)) == 0)   // Si es 'y' vuelvo a leer
        {
            rd = read(0, buffer, T);
        }
        else if(strncmp(respuesta, n, strlen(n)) == 0)
        {
            // Cierro ambas fifos
            if(close(fd) < 0)
            {
                perror("close f1 escritor");
                exit(1);
            }
            if(close(fd2) < 0)
            {
                perror("close f2 escritor");
                exit(1);
            }
            break;
        }
    }
    if(rd < 0)
    {
        perror("read escritor teclado");
        exit(1);
    }

    return 0;
}

/*  -- INCLUYO EL PROGRAMA DEL LECTOR AQUI --

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define T 32
#define limit 20

int main()
{
    // CODIGO PROGRAMA LECTOR
    // Abro fifo de lectura (f1)
    int fd;
    if((fd = open("f1", O_RDONLY)) < 0)
    {
        perror("open f1");
        exit(1);
    }
    // Abro fifo de respuesta (f2)
    int fd2;
    if((fd2 = open("f2", O_WRONLY)) < 0)
    {
        perror("open f2");
        exit(1);
    }

    // Leo de la fifo
    int rd, wr;
    char buffer[T];
    int byte_counter = 0;
    rd = read(fd, buffer, T);

    while(rd > 0)
    {
        if((wr = write(1, buffer, rd)) < 0) // Escribo por pantalla
        {
            perror("write lector pantalla");
            exit(1);
        }
        byte_counter += rd;
        
        if(byte_counter < limit)
        {
            if((wr = write(fd2, "y", 1)) < 0)
            {
                perror("write 'y' lector");
                exit(1);
            }
            rd = read(fd, buffer, T);         // Leo de nuevo
        }
        else if(byte_counter >= limit)
        {
            if((wr = write(fd2, "n", 1)) < 0)
            {
                perror("write 'n' lector");
                exit(1);
            }
            break;
        }
        
    }
    if(rd < 0)
    {
        perror("read");
        exit(1);
    }

    if(close(fd) < 0)     // Cierro el extremo de lectura 
    {
        perror("close f2 lector");
        exit(1);
    }

    if(close(fd2) < 0)
    {
        perror("close fd2 lector");
        exit(1);
    }

    
    return 0;
}

*/