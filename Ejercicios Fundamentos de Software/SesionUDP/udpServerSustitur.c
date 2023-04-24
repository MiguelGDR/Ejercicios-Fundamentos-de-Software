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

void sustituir(char *input, char *output);

int main(int argc, char *argv[])
{
    // Creamos la direcion del servidor
    struct sockaddr_in dir_servidor;
    memset(&dir_servidor, 0, sizeof(dir_servidor));

    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(PUERTO);
    dir_servidor.sin_addr.s_addr = INADDR_ANY;

    // Creamos el socket
    int sd = socket(PF_INET, SOCK_DGRAM, 0);

    if(sd < 0)
    {
        perror("socket");
        exit(1);
    }

    // Vinculamos el socket al puerto
    if((bind(sd, (struct sockaddr*) &dir_servidor, sizeof(dir_servidor))) < 0)
    {
        perror("bind");
        exit(1);
    }

    int leidos;
    char buffer[T];

    struct sockaddr_in dir_remitente;
    socklen_t lon_dir_remitente = sizeof(dir_remitente);

    // Iteramos indefinidamente
    while(1)
    {
        memset(buffer, '\0', T);
        printf("Servidor esperando mensaje...\n");
  
        if((leidos = recvfrom(sd, buffer, T-1, 0, (struct sockaddr*) &dir_remitente, &lon_dir_remitente)) < 0)
        {
            perror("rcvfrom");
            exit(1);
        }
        printf("Paquete recibido de %s:%d. \n", inet_ntoa(dir_remitente.sin_addr), ntohs(dir_remitente.sin_port));
        buffer[T] = '\0';
        printf("El paquete es: %s (%d).\n", buffer, leidos);

        // Hacemos la transformacion
        char salida[T];
        sustituir(buffer, salida);

        // Enviamos la información al cliente
        if((sendto(sd, salida, strlen(salida), 0, (struct sockaddr*) &dir_remitente, sizeof(dir_remitente))) < 0)
        {
            perror("sendto()");
            exit(1);
        }
        memset(salida, '\0', T);
    }

    if(close(sd) < 0)
    {
        perror("close");
        exit(1);
    }

    return 0;
}

void sustituir(char *input, char *output)
{
    while (*input != '\0')
    {
        if (*input >= 48 && *input <= 57)
        {
            *output = '|';
            output += 1;

            for (int i = 1; i <= (*input - 48); i++)
            {
                *output = '*';
                output += 1;
            }

            *output = '|';
            output += 1;
            input += 1;
        }
        else
        {
            *output = *input;
            input += 1;
            output += 1;
        }
    }
    *output = *input;
}
