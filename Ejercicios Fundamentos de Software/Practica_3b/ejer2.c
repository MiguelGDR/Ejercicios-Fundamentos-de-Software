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
    int p1[2], p2[2];           // p1 used as father to son | p2 used as son to father 

    if(pipe(p1) < 0)
    {
        perror("pipe1");
        exit(1);
    }
    if(pipe(p2) < 0)
    {
        perror("pipe2");
        exit(1);
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
        // Hijo
        if(close(p1[1]) < 0)                 // Cierro extremo escritura en el hijo
        {
            perror("close escritura hijo del pipe1");
            exit(1);
        }
        if(close(p2[0]) < 0)
        {
            perror("close lectura hijo pipe2");
            exit(1);
        }

        // Leo del pipe
        int rd, wr;
        char buffer[T];
        int byte_counter = 0;   // Usado para contar los bytes leidos y compararlos con el limite
        rd = read(p1[0], buffer, T);

        while(rd > 0)
        {
            if((wr = write(1, buffer, rd)) < 0) // Escribo por pantalla
            {
                perror("write hijo pantalla");
                exit(1);
            }
            byte_counter += rd;
            
            if(byte_counter < limit)
            {
                if((wr = write(p2[1], "y", 1)) < 0)
                {
                    perror("write 'y' hijo");
                    exit(1);
                }
                rd = read(p1[0], buffer, T);         // Leo de nuevo
            }
            else if(byte_counter >= limit)
            {
                if((wr = write(p2[1], "n", 1)) < 0)
                {
                    perror("write 'n' hijo");
                    exit(1);
                }
                break;
            }
        }
        if(rd < 0)
        {
            perror("read hijo");
            exit(1);
        }

        if(close(p1[0] < 0))     // Cierro el extremo de lectura del hijo
        {
            perror("close lectura hijo");
            exit(1);
        }
        if(close(p2[1]) < 0)
        {
            perror("close escritura hijo");
            exit(1);
        }
        
        exit(0);

    }
    else
    {
        // Padre
        if(close(p1[0]) < 0)                 // Cierro extremo lectura en el padre de la pipe1
        {
            perror("close lectura padre del pipe1");
            exit(1);
        }
        if(close(p2[1]) < 0)              // Cierro el extremo de escritura de la pipe2
        {
            perror("close escritura padre del pipe2");
            exit(1);
        }

        // Leo del teclado
        int rd, wr;  
        char respuesta[1];         // Para comprobar la respuesta del hijo 'y' o 'n'
        char y[] = "y", n[] = "n";
        char buffer[T];
        rd = read(0, buffer, T);        

        while(rd > 0)
        {
            if((wr = write(p1[1], buffer, rd)) < 0)  // Escribo en la pipe menos el enter
            {
                perror("write padre pipe");
                exit(1);
            }
            if((rd = read(p2[0], respuesta, 1)) < 0)  // Espero respuesta del hijo
            {
                perror("read respuesta de hijo a padre");
                exit(1);
            }

            // Compruebo la respuesta
            if(strncmp(respuesta, y, strlen(y)) == 0)   // Si es 'y' vuelvo a leer
            {
                rd = read(0, buffer, T);
            }
            else if(strncmp(respuesta, n, strlen(n)) == 0)  // Si es 'n' cierro ambos canales
            {
                // Cierro ambos canales
                if(close(p2[0]) < 0)
                {
                    perror("close lectura padre");
                    exit(1);
                }
                if(close(p1[1]) < 0)
                {
                    perror("close escritura padre");
                    exit(1);
                }
                break;
            }
        }
        if(rd < 0)
        {
            perror("read padre teclado");
            exit(1);
        }

        wait(0);
    }


}