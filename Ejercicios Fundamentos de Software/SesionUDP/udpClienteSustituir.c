#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define PUERTO 4950
#define T 1024

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("Uso: %s <ip> <puerto> <mensaje>.\n", argv[0]);
        exit(1);
    }

    // Definir la dirección donde está el servidor
    struct sockaddr_in dir_servidor;
    memset(&dir_servidor, 0, sizeof(dir_servidor));
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(atoi(argv[2]));
    uint32_t ip = inet_addr(argv[1]);   // Convierte la direccion con puntos en direccion big endian
    if(ip < 0)
    {
        printf("IP incorrecta: %s\n", argv[1]);
        exit(1);
    }

    dir_servidor.sin_addr.s_addr = ip;

    // Creamos el socket
    int sd = socket(PF_INET, SOCK_DGRAM, 0); // Protocol Family Internet, Socket Datagram, 0 = UDP
    if(sd < 0)
    {
        perror("socket");
        exit(1);
    }

    // Al ser un servicio no orientado a conexión, 
    // ya podemos enviar información
    int enviados = sendto(sd, argv[3], strlen(argv[3]), 0, (struct sockaddr*) &dir_servidor, sizeof(dir_servidor));    
                    // sd - desciptor del socket.
                    // argv[3] - puntero dir memoria donde estan los datos a enviar. 
                    // strlen() - tamaño de la cadena
                    // 0 - op de envio
                    // &dir_servidor - puntero a direccion del servidor
                    // sizeof(dir_servidor) - tamaño de la direccion del servidor

    if(enviados < 0)
    {
        perror("sendto()");
        exit(1);
    }
    printf("Enviados %d bytes a %s\n", enviados, inet_ntoa(dir_servidor.sin_addr));

    int rd;
    char buffer[T];
    struct sockaddr_in dir_remitente;
    socklen_t long_dir_remitente = sizeof(dir_remitente);

    rd = recvfrom(sd, buffer, T - 1, 0, (struct sockaddr*) &dir_remitente, &long_dir_remitente);
    buffer[T] = '\0';

    printf("Mensaje recibido: %s, (%d)\n", buffer, rd);

    if(rd < 0)
    {
        perror("rcvfrom");
        exit(1);
    }

    // Cerramos el socket
    if(close(sd) < 0)
    {
        perror("close(sd)");
        exit(1);
    }

    return 0;
}