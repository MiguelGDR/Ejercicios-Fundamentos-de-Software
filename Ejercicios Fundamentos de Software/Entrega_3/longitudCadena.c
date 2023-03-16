#include <stdio.h>
#define T 32

int longitudCadena(char c[])
{
    int i = 0;
    while(c[i] != '\0')
    {
        i++;
    }
    return i;
}

int main()
{
    int l = longitudCadena("hola"); // se espera que l = 4
    printf("%d\n", l);

    l = longitudCadena(""); // se espera que l = 0
    printf("%d\n", l);

    return 0;
}