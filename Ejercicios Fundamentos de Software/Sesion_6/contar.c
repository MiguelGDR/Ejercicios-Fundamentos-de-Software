#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define T 1

void contar(char * filename, int * minusculas, int * mayusculas, int * digitos)
{
    int fd;
    *minusculas = 0;
    *mayusculas = 0;
    *digitos = 0;

    // Abro fichero
    if((fd = open(filename, O_RDONLY)) < 0) 
    {
        perror("open");
        exit(1);
    }

    // Leo fichero
    char b[T];
    int leidos;
    leidos = read(fd,b,T);
    while(leidos > 0)
    {
        for(int i = 0; i < T; i++)
        {
            if(b[i] >= 65 && b[i] <= 90)    // Si es mayúscula
            {
                *mayusculas += 1;
            }else if(b[i] >= 97 && b[i] <= 122) // Si es minúscula
            {
                *minusculas += 1;
            }
            else if(b[i] >= 48 && b[i] <= 57)   // Si es digito
            {
                *digitos += 1;
            }
        }
        leidos = read(fd, b, T);
    }
    if(leidos < 0)
    {
        perror("read");
        exit(1);
    }

    // Cierro fichero
    if(close(fd) < 0)
    {
        perror("close");
        exit(1);
    }
}

int main()
{
    //llamada a la función
    int minusculas, mayusculas, digitos;
    contar("datos.txt", &minusculas, &mayusculas, &digitos);

    printf("minusculas = %d, mayusculas = %d, digitos = %d\n", minusculas, mayusculas, digitos);
    //se espera que minusculas = 7; mayusculas = 4; digitos = 4;
}