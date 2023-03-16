#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define T 512

struct Estructura
{
    int a;
    float b;
    char c;
};

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("Falta el <filename>.\n");
        exit(1);
    }

    int x = 7;
    int array_enteros[4] = {0x00, 0x01, 0x02, 0x03};
    
    struct Estructura est;
    est.a = 1; 
    est.b = 2.0;       
    est.c = '3'; 

    char buffer[T];
    memcpy(buffer, &x, sizeof(int));
    memcpy((buffer + sizeof(int)), &array_enteros, (sizeof(int) * 4));
    memcpy((buffer + (sizeof(int) * 5) ), &est, (sizeof(int) + sizeof(float) + sizeof(char)));
    
    
    int fd;     // Descriptor
    
    if ((fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)   // Abro el fichero
    {
        perror("open");
        exit(1); 
    }

    int wr; 

    if((wr = write(fd, buffer, ( (sizeof(int) * 6) + sizeof(float) + sizeof(char) ))) < 0)
    {
        perror("error");
        exit(1);
    }

    if(close(fd) < 0)
    {
        perror("close");
        exit(1);
    }

    return 0;
}