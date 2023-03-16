#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define T 256   // Defino un tamaño fijo para ir leyendo el contenido y copiando

int copiaFichero(char *forigen, char *fdestino)
{
    int fd_ori, fd_des;

    // Creo el buffer
    char buffer[T];

    // Abro ambos ficheros
    // El primero como lectura
    if((fd_ori = open(forigen, O_RDONLY)) < 0)
    {
        perror("open origen");
        exit(1);
    }
    // El segundo como escritura
    if((fd_des = open(fdestino, O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
    {
        perror("open destino");
        exit(1);
    }

    // Leo y escribo

    int rd, wr;

    rd = read(fd_ori, buffer, T);

    while(rd > 0)
    {
        if((wr = write(fd_des, buffer, rd)) < 0)    // Escribo la informacion leida
        {
            perror("write destino");
            exit(1);
        }

        rd = read(fd_ori, buffer, T);
    }

    if(rd < 0)
    {
        perror("read origen");
        exit(1);
    }

    // Cierro los ficheros
    if(close(fd_ori) < 0)
    {
        perror("close origen");
        exit(1);
    }
    if(close(fd_des) < 0)
    {
        perror("close destino");
        exit(1);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Hay que incluir los nombres de: <fichero_a_copiar> <fichero_copiado>");
        exit(1); 
    }

    copiaFichero(argv[1], argv[2]);

    return 0;
}