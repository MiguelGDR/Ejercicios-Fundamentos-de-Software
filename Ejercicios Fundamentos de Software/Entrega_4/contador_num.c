#include <stdio.h>
#include <stdlib.h>

#define T 32

void contador(char c[])
{
    int i = 0;
    int contador[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    while (c[i] != '-')
    {
        if (c[i] == ' ')
        {
            i++;
        }
        else if(c[i] == '0')
        {
            contador[(0)] += 1;
            i++;
        }
        else
        {
            contador[(c[i] - '0')] += 1;
            i++;
        }
    }
    for (int i = 0; i <= 9; i++)
    {
        printf("%d ", i);
        for (int e = 0; e < contador[i]; e++)
        {
            printf("*");
        }
        printf("\n");
    }
}

int main()
{
    char c[T];

    printf("Introduzca una secuencia de numeros entre 0 y 9 terminada en numero negativo: ");
    // scanf("%s", c); // Guarda la cadena de caracteres en la variable c.
    gets(c);
    
    contador(c);

    return 0;
}