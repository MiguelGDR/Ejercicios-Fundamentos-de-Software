#include <stdio.h>
#include <unistd.h>
#define T 8

int main()
{
    char b[T];
    printf("Introduzca una cadena: \n");
    int leidos = read(0, b, T);

    printf("s = %s\n", b);

    return 0;
}
