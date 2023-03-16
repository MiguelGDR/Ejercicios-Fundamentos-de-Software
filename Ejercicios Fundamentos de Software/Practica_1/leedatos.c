#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct Estructura
{
    int a;
    float b;
    char c;
};

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Falta el <filename> del fichero a leer.\n");
        exit(1);
    }
    int fd;     // Inicio descriptor

    // Abro fichero
    if ( (fd = open(argv[1], O_RDONLY)) < 0)    
    {
        perror("open");
        exit(1);
    }

    int rd;
    int buffer_size = (sizeof(int) * 6) + (sizeof(float)) + (sizeof(char));
    char buffer[buffer_size];

    // Leo fichero
    if((rd = read(fd, buffer, buffer_size)) < 0)
    {
        perror("read");
        exit(1);
    }   
     
    // Asigno el contenido del buffer a cada variable
    // y la muestro por pantalla
    int x;
    memcpy(&x,buffer,sizeof(int));
    printf("Entero: %d\n", x);

    int array_enteros[4];
    memcpy(&array_enteros, buffer + sizeof(int), sizeof(int) * 4);
    printf("Array: %d, %d, %d, %d\n", array_enteros[0], array_enteros[1], array_enteros[2], array_enteros[3]);

    struct Estructura est;
    memcpy(&est, buffer + (sizeof(int) * 5), (sizeof(int) + sizeof(float) + sizeof(char)));
    printf("Estructura: %d, %f, %s\n", est.a, est.b, &est.c);
    
    
    // Cierro fichero
    if (close(fd))
    {
        perror("close");
        exit(1);
    }

    return 0;
}