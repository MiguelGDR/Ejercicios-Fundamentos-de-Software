#include <stdio.h>

#define T 32

void concatenar(char c1[], char c2[])
{
    int i = 0;
    while (c1[i] != '\0')
    {
        i++;
    } // Al salir del bucle, tengo en i el valor de la posición donde se encuentra el '\0'.
      // Ahi mismo tengo que empezar a añadir los caracteres del siguiente array (c2).
    int e = 0;
    while (c2[e] != '\0')
    {
        c1[i] = c2[e];
        i++;
        e++;
    } // Se han concatenado los caracteres, pero falta añadir el '\0'.

    c1[i] = c2[e]; // Añado el '\0'.

    return;
}

int main()
{
    char c1[32] = "hola";
    char c2[32] = "adios";

    concatenar(c1, c2); // se espera que c1 = "holaadios";

    printf("%s\n", c1);
    printf("%s\n", c2);

    char c3[32] = "";
    char c4[32] = "adios";
    concatenar(c3, c4); // se espera que c1 = "adios";
    printf("%s\n", c3);
    printf("%s\n", c4);

    return 0;
}