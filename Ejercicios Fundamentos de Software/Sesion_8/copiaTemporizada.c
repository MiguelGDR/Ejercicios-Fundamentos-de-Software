#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define T 8   // Defino un tamaño fijo para ir leyendo el contenido y copiando

// Variables globales
int fd_ori, fd_des;
char buffer[T];

void m(int signo)   // Al llegar la señal
{
    sleep(1);       // Espero 1 segundo

    int wr;

    // Escribo en el fichero destino el buffer
    if((wr = write(fd_des, buffer, T)) < 0) 
    {
        perror("write destino");
        exit(1);
    }

    signal(SIGUSR1, m);     // Preparo el manejador de nuevo
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Uso: %s <forigen> <fdestino>.\n", argv[0]);
        exit(1); 
    }

    signal(SIGUSR1, m);

    // Abro los ficheros
    if((fd_ori = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open origen");
        exit(1);
    }
    if((fd_des = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
    {
        perror("open destino");
        exit(1);
    }

    int rd;

    // Leo el fichero origen
    rd = read(fd_ori, buffer, T);

    while(rd > 0)
    {   
        pause();                            // Espero a la señal           
        rd = read(fd_ori, buffer, T);       // Vuelve a leer despues del manejador
    }

    if(rd < 0)
    {
        perror("read origen");
        exit(1);
    }

    // Cierro ficheros
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