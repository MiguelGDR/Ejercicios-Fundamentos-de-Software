#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>

#define T 32

/**
 * Funciones auxiliares
 */
ssize_t read_n(int fd, void * mensaje, size_t longitud_mensaje) {
  ssize_t a_leer = longitud_mensaje;
  ssize_t total_leido = 0;
  ssize_t leido;
  
  do {
    errno = 0;
    leido = read(fd, mensaje + total_leido, a_leer);
    if (leido >= 0) {
      total_leido += leido;
      a_leer -= leido;
    }
  } while((
      (leido > 0) && (total_leido < longitud_mensaje)) ||
      (errno == EINTR));
  
  if (total_leido > 0) {
    return total_leido;
  } else {
    /* Para permitir que devuelva un posible error en la llamada a read() */
    return leido;
  }
}

ssize_t write_n(int fd, void * mensaje, size_t longitud_mensaje) {
  ssize_t a_escribir = longitud_mensaje;
  ssize_t total_escrito = 0;
  ssize_t escrito;
  
  do {
    errno = 0;
    escrito = write(fd, mensaje + total_escrito, a_escribir);
    if (escrito >= 0) {
      total_escrito += escrito ;
      a_escribir -= escrito ;
    }
  } while(
        ((escrito > 0) && (total_escrito < longitud_mensaje)) ||
        (errno == EINTR));
  
  if (total_escrito > 0) {
    return total_escrito;
  } else {
    /* Para permitir que devuelva un posible error de la llamada a write */
    return escrito;
  }
}

// FUNCION PARA SUSTITUIR DIGITOS POR |**|
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


// FUNCION PARA HACER MINUSCULAS 
void minus(char *input, char *output, int rd)
{
    for(int i = 0; i < rd; i++) 
    {
        *output = tolower(*input);
        output += 1;
        input += 1;
    }
}


// MAIN
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Uso: %s <data1.txt> <data2.txt>.\n", argv[0]);
        exit(1);
    }

    int p[2];   // p[0] extremo lectura, p[1] extremo escritura

    if(pipe(p) < 0)
    {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if( pid == 0)
    {
        // Hijo 
        if(close(p[1]) < 0)     // Cierro extremo de escritura
        {
            perror("close p[1] hijo");
            exit(1);
        }
// ----------------------------------------------------------------------------
        // Abro el fichero de escritura
        int fd1_wr;
        char f2_filter[strlen(argv[1]) + 3];
        sprintf(f2_filter, "%s.f2", argv[1]);

        if((fd1_wr = open(f2_filter, O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
        {
            perror("open fd2_wr");
            exit(1);
        }
// ----------------------------------------------------------------------------
        // Leo del pipe
        char leido[T], filtrado[T];
        int tam, rd, wr;

        if((rd = read_n(p[0], &tam, sizeof(int))) < 0)    // Leo el tamaño
        {
            perror("read_n tamaño");
            exit(1);
        }

        while(rd > 0)
        {
            if(tam == 0)
            {
                rd = 0;
            }
            else
            {
                rd = read_n(p[0], leido, tam);      // Leo el contenido del pipe
                sustituir(leido, filtrado);
                if((wr = write_n(fd1_wr, filtrado, strlen(filtrado))) < 0)   // Escribo en el fichero 
                {
                    perror("write data1.txt.f2");
                    exit(1);
                }

                if((rd = read_n(p[0], &tam, sizeof(int))) < 0)    // Leo el tamaño
                {
                    perror("read_n tamaño");
                    exit(1);
                }
            }

        }
        if(rd < 0)
        {
            perror("read pipe[0] - hijo");
            exit(1);
        }
// ----------------------------------------------------------------------------
        if(close(fd1_wr) < 0)   // Cierro el fichero data1.txt.f2
        {
            perror("close data1.txt.f2");
            exit(1);
        }
// ----------------------------------------------------------------------------
        // Abro el fichero data2.txt.f2
        int fd2_wr;
        char f2_filter2[strlen(argv[2]) + 3];
        sprintf(f2_filter2, "%s.f2", argv[2]);

        if((fd2_wr = open(f2_filter2, O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
        {
            perror("open fd2_wr");
            exit(1);
        }
        // Leo del pipe
        if((rd = read_n(p[0], &tam, sizeof(int))) < 0)    // Leo el tamaño
        {
            perror("read_n tamaño");
            exit(1);
        }

        while(rd > 0)
        {
            if(tam == 0)
            {
                rd = 0;
            }
            else
            {
                rd = read_n(p[0], leido, tam);      // Leo el contenido del pipe
                sustituir(leido, filtrado);
                if((wr = write_n(fd2_wr, filtrado, strlen(filtrado))) < 0)   // Escribo en el fichero 
                {
                    perror("write data2.txt.f2");
                    exit(1);
                }

                if((rd = read_n(p[0], &tam, sizeof(int))) < 0)    // Leo el tamaño
                {
                    perror("read_n tamaño");
                    exit(1);
                }
            }

        }
        if(rd < 0)
        {
            perror("read pipe[0] - hijo");
            exit(1);
        }
// ----------------------------------------------------------------------------
        if(close(fd2_wr) < 0)   // Cierro el fichero data2.txt.f2
        {
            perror("close data1.txt.f2");
            exit(1);
        }
        if(close(p[0]) < 0)     // Cierro extremo de lectura
        {
            perror("close p[0] hijo");
            exit(1);
        }

        exit(0);
    }
    else
    {
        // Padre
        if(close(p[0]) < 0)     // Cierro extremo de lectura
        {
            perror("close pipe lectura padre");
            exit(1);
        }
// ----------------------------------------------------------------------------
        int fd1_rd, fd1_wr;

        // Abro el fichero 1
        if((fd1_rd = open(argv[1], O_RDONLY)) < 0)
        {
            perror("open data1.txt");
            exit(1);
        }
        // Abro el fichero de escritura 1
        char f1_filter[strlen(argv[1]) + 3];
        sprintf(f1_filter, "%s.f1", argv[1]);

        if((fd1_wr = open(f1_filter, O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
        {
            perror("open data1.txt.f1");
            exit(1);
        }
// ----------------------------------------------------------------------------
        char leido[T], filtrado[T];
        int tam,rd, wr;
        // Leo fichero
        rd = read(fd1_rd, leido, T);
        tam = rd;
        while(rd > 0)
        {
            minus(leido, filtrado, rd);
            if((wr = write_n(fd1_wr, filtrado, rd)) < 0)     // Escribo con filtro 1
            {
                perror("write data1.txt.f1");
                exit(1);
            }
            if((wr = write_n(p[1], &tam, sizeof(int))) < 0)      // Escribo el tamaño de lo que viene despues
            {
                perror("write p[1] - tamaño padre");
                exit(1);
            }
            if((wr = write_n(p[1], filtrado, tam)) < 0)          // Escribo el contenido
            {
                perror("write p[1] - contenido padre");
                exit(1);
            }
            rd = read(fd1_rd, leido, T);
        }
        if(rd < 0)
        {
            perror("read data1.txt");
            exit(1);
        }else if(rd == 0)       // Para hacer saber al destino que se acabo el fichero 1
        {
            if((wr = write_n(p[1], &rd, sizeof(int))) < 0)
            {
                perror("write p[1] - 0");
                exit(1);
            }
        }
// ----------------------------------------------------------------------------
        // Cierro ambos ficheros
        if(close(fd1_rd) < 0)
        {
            perror("close data1.txt");
            exit(1);
        }
        if(close(fd1_wr) < 0)
        {
            perror("close data1.txt.f1");
            exit(1);
        }
// ----------------------------------------------------------------------------
        // Lo mismo pero con el fichero 2
        // Abro el fichero 2
        int fd2_rd, fd2_wr;

        if((fd2_rd = open(argv[2], O_RDONLY)) < 0)
        {
            perror("open data2.txt");
            exit(1);
        }
        // Abro el fichero de escritura 2
        char f2_filter[strlen(argv[2]) + 3];
        sprintf(f2_filter, "%s.f1", argv[2]);

        if((fd2_wr = open(f2_filter, O_WRONLY | O_TRUNC | O_CREAT, 0640)) < 0)
        {
            perror("open data2.txt.f1");
            exit(1);
        }
// ----------------------------------------------------------------------------
        // Leo fichero
        rd = read(fd2_rd, leido, T);
        tam = rd;
        while(rd > 0)
        {
            minus(leido, filtrado, rd);
            if((wr = write_n(fd2_wr, filtrado, rd)) < 0)     // Escribo con filtro 1
            {
                perror("write data2.txt.f1");
                exit(1);
            }
            if((wr = write_n(p[1], &tam, sizeof(int))) < 0)      // Escribo el tamaño de lo que viene despues
            {
                perror("write p[1] - tamaño padre");
                exit(1);
            }
            if((wr = write_n(p[1], filtrado, tam)) < 0)          // Escribo el contenido
            {
                perror("write p[1] - contenido padre");
                exit(1);
            }
            rd = read(fd2_rd, leido, T);
        }
        if(rd < 0)
        {
            perror("read data2.txt");
            exit(1);
        }
// ----------------------------------------------------------------------------
        // Cierro ambos ficheros
        if(close(fd2_rd) < 0)
        {
            perror("close data2.txt");
            exit(1);
        }
        if(close(fd2_wr) < 0)
        {
            perror("close data2.txt.f1");
            exit(1);
        }
// ----------------------------------------------------------------------------

        if(close(p[1]) < 0)     // Cierro extremo de escritura
        {
            perror("close");
            exit(1);
        }

        wait(0);
    }

    return 0;
}
