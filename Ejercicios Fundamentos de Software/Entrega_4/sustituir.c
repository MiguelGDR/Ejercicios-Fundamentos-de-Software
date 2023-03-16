#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 32

void sustituir(char *input, char **output)
{
    while (*input != '\0')
    {
        if (*input >= 48 && *input <= 57)
        {
            printf("|");
            for (int i = 0; i <= (*input - 48); i++)
            {
                printf("*");
            }
            printf("|");
        }
        else
        {
            **output = *input;
            input += 1;
            *output += 1;
        }
    }
}

int main()
{
    char c[T] = "hola";
    char **salida;

    /*printf("Introduzca una cadena: ");
    gets(c);*/

    sustituir(c, salida);

    printf("%s", **salida);

    return 0;
}