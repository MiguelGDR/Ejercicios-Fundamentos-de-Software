#include <stdio.h>
#include <math.h>

int taylor_serie_geometrica(double *v, double x, int orden)
{
    *v = 0; // Inicializo *v a 0;
    if (x >= 1 || x <= -1)
    {
        return -1; // Si x no esta en el intervalo [-1,1], v pasa a valer -1.
    }
    else
    {
        for (double i = 0; i <= orden; i++) // Si x si esta en el intervalo [-1,1], se realiza el cálculo.
        {
            *v += pow(x, i);
        }
    }
    return 0;
}

int main()
{
    double v;
    int r = taylor_serie_geometrica(&v, 1.233099, 0);
    printf("r = %d\n", r);
    // Salida esperada: r = -1;

    r = taylor_serie_geometrica(&v, 0.329167, 2);
    printf("v = %f\n", v);
    // Salida esperada: v = 1.437518;

    r = taylor_serie_geometrica(&v, -0.510229, 4);
    printf("v = %f\n", v);
    // Salida esperada: v = 0.685049;

    return 0;
}
