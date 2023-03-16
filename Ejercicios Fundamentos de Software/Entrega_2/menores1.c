#include <stdio.h>
#define T 21

int sumaMenores(int n, int a[T], int size)
{
    int suma = 0;

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < (size - 1); i++)
        {
            if (a[i] < 0 || n > size)
            {
                return -1;
            }
            else if (a[i] > a[i + 1])
            {
                int aux = a[i + 1];
                a[i + 1] = a[i];
                a[i] = aux;
            }
        }
    }
    for (int e = 0; e < n; e++)
    {
        suma += a[e];
    }
    
    return suma;
}

int main()
{
    int a[T] = {2, 106, 93, 145, 78, 62, 190, 42, 142, 26, 192, 91, 82, 47, 164, 111, 36, 71, 168, 61, 23};
    int m = sumaMenores(12, a, 21);
    // se espera que m = 621
    printf("%d\n", m);

    int a2[T] = {201, 77, 62, 87, 42, 202, 176, -76, 170, 146, 181, 105, 209, 34, 29, 40, 129, 171, 67, 87, 11};
    m = sumaMenores(5, a2, 21);
    // se espera que m = -1
    printf("%d\n", m);

    int a3[T] = {201, 77, 62, 87, 42, 202, 176, -76, 170, 146, 181, 105, 209, 34, 29, 40, 129, 171, 67, 87, 11};
    m = sumaMenores(15, a3, 10);
    // se espera que m = -1
    printf("%d\n", m);

    return 0;
}