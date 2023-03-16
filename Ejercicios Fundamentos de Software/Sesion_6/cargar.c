#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define T 512

void cargar(char *filename)
{
    // Inicio el descriptor
    int fd;

    // Abro el fichero para lectura
    if((fd = open(filename, O_RDONLY)) < 0)
    {
        perror("open");
        exit(1);
    }

    // Inicio el buffer y datos a guardar
    char buffer[T];
    short pdu_size;
    char pdu_payload[T];
    long pdu_crc;

    // Inicio wr
    int wr;

    // Leo del fichero
    // Primero leo el short con el tamaño del size
    
    int contador_pdu = 1;

    wr = read(fd, buffer, sizeof(short));

    while(wr > 0)
    {
        if(wr < 0)
        {
            perror("write");
            exit(1);
        }
        memcpy(&pdu_size, buffer, sizeof(short));

        // Leo el payload en funcion del size leido
        wr = read(fd, buffer, (sizeof(char) * pdu_size));
        if(wr < 0)
        {
            perror("write");
            exit(1);
        }
        memcpy(pdu_payload, buffer, (sizeof(char) * pdu_size)); // Guardo el payload
        memcpy(&pdu_payload[pdu_size], "\0", sizeof(char));

        // Leo el CRC del final
        pdu_crc = 0;

        wr = read(fd, buffer, sizeof(long));
        if(wr < 0)
        {
            perror("write");
            exit(1);
        }
        memcpy(&pdu_crc, buffer, sizeof(long));

        // Calculo CRC
        int cal_crc = 0;
        for(int i = 0; i < pdu_size; i++)
        {
            cal_crc += pdu_payload[i];
        }

        // Compruebo CRC
        if(pdu_crc == cal_crc)
        {
            printf("PDU %d: {size = %d,payload = %s,crc = %ld}\n", contador_pdu, pdu_size, pdu_payload, pdu_crc);
        }
        else
        {
            printf("PDU %d: corrupta", contador_pdu);
        }

        contador_pdu += 1;
        wr = read(fd, buffer, sizeof(short));
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Faltan argumentos: <filename>.\n");
        exit(1);
    }

    cargar(argv[1]);
    
    return 0;
}