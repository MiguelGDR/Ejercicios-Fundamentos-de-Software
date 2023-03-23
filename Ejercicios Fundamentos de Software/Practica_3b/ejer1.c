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
    int p[2];

    if(pipe(p) < 0)
    {
        perror("pipe");
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
        if(close(p[1]) < 0)                 // Cierro extremo escritura en el hijo
        {
            perror("close escritura hijo");
            exit(1);
        }

        // Leo del pipe
        int rd, wr;
        char buffer[T];
        rd = read(p[0], buffer, T);

        while(rd > 0)
        {
            if((wr = write(1, buffer, rd)) < 0) // Escribo por pantalla
            {
                perror("write hijo pantalla");
                exit(1);
            }

            rd = read(p[0], buffer, T);         // Leo de nuevo
        }
        if(rd < 0)
        {
            perror("read hijo");
            exit(1);
        }
        if(rd == 0) // Se cerró la pipe
        {
            if(close(p[0] < 0))     // Cierro el extremo de lectura del hijo
            {
                perror("close lectura hijo");
                exit(1);
            }
        }
        exit(0);

    }
    else
    {
        // Padre
        if(close(p[0]) < 0)                 // Cierro extremo lectura en el padre
        {
            perror("close lectura padre");
            exit(1);
        }

        // Leo del teclado
        int rd, wr;
        char buffer[T];
        char fin[] = "fin\n";
        rd = read(0, buffer, T);        

        while(rd > 0)
        {
            if(strncmp(buffer, fin, strlen(fin)) == 0)    // Si se escribe "fin\n", se cierra el extremo de escritura del padre
            {
                if(close(p[1]) < 0)                 // Cierro extremo escritura padre (envia 0 al hijo por el pipe)
                {
                    perror("close escritura padre");
                    exit(1);
                }

                break;
            }       
            else    // Si no, transmito la informacion por el pipe al hijo
            {
                if((wr = write(p[1], buffer, rd)) < 0)  // Escribo en la pipe menos el enter
                {
                    perror("write padre pipe");
                    exit(1);
                }

                rd = read(0, buffer, T);
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