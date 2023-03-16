#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);
sighandler_t puntero_a_funcion;

void manejador2(int s);

void manejador1(int s)
{
    write(1, "manejador1\n", 11);
    puntero_a_funcion = signal(SIGINT, manejador2);
}

void manejador2(int s)
{
    write(1, "manejador2\n", 11);
    puntero_a_funcion = signal(SIGINT, manejador1);
}

int main()
{
    signal(SIGINT, manejador1);

    while(1)
    {
        pause();
    }
    return 0;
}