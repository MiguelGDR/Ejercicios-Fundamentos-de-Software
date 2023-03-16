#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 32

void eliminarVocales(char *c)
{
    char *aux = c; // Para recorrer el array
    char *store = c; // Guardo el puntero a la primera pos

    while (*c != '\0')
    {
        if (*aux == 'a' || *aux == 'e' || *aux == 'i' || *aux == 'o' || *aux == 'u')
        {
            aux = aux + 1; // Si es vocal, paso a la siguiente posicion
        }
        else
        {
            *c = *aux; // Si es consonante, la guardo en la cadena c.
            c += 1;
            aux += 1;
        }
    }
    c = store;
}

int main()
{
    char c[T] = "hola y adios";
    eliminarVocales(c);

    char c1[T] = "aqwfubgue";
    eliminarVocales(c1);
}