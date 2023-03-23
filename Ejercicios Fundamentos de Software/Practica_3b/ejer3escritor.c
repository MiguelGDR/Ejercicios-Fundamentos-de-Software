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
    // Abro la fifo para escribir
    int fd;
    if((fd = open("f1", O_WRONLY)) < 0)
    {
        perror("wrtie f1");
        exit(1);
    }
    // Leo del teclado
    int rd, wr;
    char buffer[T];
    char fin[] = "fin\n";
    rd = read(0, buffer, T);        

    while(rd > 0)
    {
        if(strncmp(buffer, fin, strlen(fin)) == 0)    // Si se escribe "fin\n", se cierra el extremo de escritura de la fifo
        {
            if(close(fd) < 0)                 // Cierro extremo escritura (envia 0 al lector por la fifo)
            {
                perror("close fd escritor");
                exit(1);
            }
            break;
        }       
        else    // Si no, transmito la informacion por la fifo al lector
        {
            if((wr = write(fd, buffer, rd)) < 0)  // Escribo en la fifo
            {
                perror("write escritor fifo");
                exit(1);
            }

            rd = read(0, buffer, T);
        }
    }
    if(rd < 0)
    {
        perror("read escritor teclado");
        exit(1);
    }

    return 0;
}