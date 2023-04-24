#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Programa que recibe como argumento una dir IP 
// y un puerto, y crea la dir socket

int main(int argc, char *argv[])
{
    // Compruebo los argumentos
    if(argc < 3)
    {
        printf("Uso: %s, <ip> <puerto>\n", argv[0]);
        exit(1);
    }

    // Declaro la estructura
    struct sockaddr_in dir_socket;

    // Inicializo la estructura
    memset(&dir_socket, 0, sizeof(dir_socket));

    // Definimos la familia de direccion
    dir_socket.sin_family = AF_INET;

    // Asignamos el puerto
    dir_socket.sin_port = htons(atoi(argv[2]));

    // Obtenemos la dir IP
    uint32_t ip = inet_add(argv[1]);
    if(ip < 0)
    {
        printf("Dir IP no válida: %s\n", argv[1]);
        exit(1);
    }
    
    // Copiamos la dir IP en la dir de socket
    // memcpy(&dir_socket.sin_addr, &ip, 4);
    dir_socket.sin_addr.s_addr = ip;
    return 0;
}