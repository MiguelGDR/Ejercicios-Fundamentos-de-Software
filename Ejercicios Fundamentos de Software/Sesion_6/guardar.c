#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define T 512

struct PDU {
   short size;
   char * payload;
   long crc;
};

int longitudCadena(char c[])
{
    int i = 0;
    while(c[i] != '\0')
    {
        i++;
    }
    return i;
}


int guardar(char * filename, struct PDU * pdu)
{
    // Abro el fichero
    int fd;
    if((fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0640)) < 0)
    {
        perror("open");
        return -1;
    }


    // Guardar toda la informacion en el buffer 
    // como: pdu.size pdu.payload pdu.crc
    char buffer[T];
    memcpy(buffer, &pdu->size, sizeof(short));
    memcpy((buffer + sizeof(short)), pdu->payload, pdu->size);
    memcpy((buffer + sizeof(short) + pdu->size), &pdu->crc, sizeof(long));

    // Escribo el contenido del buffer al fichero
    int wr;

    wr = write(fd, buffer, (sizeof(short) + pdu->size + sizeof(long)));
    if(wr < 0)
    {
        perror("write");
        exit(1);
    }

    // Cierro el fichero
    if(close(fd) < 0)
    {
        perror("close");
        return -1;
    }
    return 0;
}

int main (int argc, char * argv[])
{
    // Se recibe como argumentos:
    // - argv[1]- primero el nombre del fichero 
    // donde se guardará el contenido.
    // - argv[2]- contenido a guardar.
    if (argc < 3)
    {
        printf("Ademas del %s, hace falta el <filename.dat> y <payload>.", argv[0]);
        exit(1);
    }

    struct PDU pdu;
    pdu.payload = argv[2];
    pdu.size = longitudCadena(argv[2]);

    int i = 0;
    pdu.crc = 0;
    while(pdu.payload[i] != '\0')
    {
        pdu.crc += pdu.payload[i];
        i++;
    }

    printf("Nombre: %s, Contenido: %s, Tamaño %d, Suma CRC: %ld\n", argv[1], pdu.payload, pdu.size,pdu.crc);
    
    guardar(argv[1], &pdu);

    return 0;
}