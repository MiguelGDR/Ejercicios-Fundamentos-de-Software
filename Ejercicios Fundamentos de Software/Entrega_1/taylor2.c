#include <stdio.h>
#include <math.h>

double taylor_serie_exp(double x, int orden)
{
    double valor = 0;
    int factorial;

    for (double i = 0; i <= orden; i++) // Si x si esta en el intervalo [-1,1], se realiza el cálculo.
    {
        if (i == 0)
        {
            factorial = 1;
        }
        else
        {
            factorial = 1;
            for (int e = 1; e <= i; e++)
            {
                factorial *= e;
            }
        }
        valor += (pow(x, i)) / factorial;
    }
    return valor;
}

int main()
{
    double v = taylor_serie_exp(-7.204568, 4);
    printf("v = %f;\n", v);
    // Salida esperada: v = 69.680709;

    v = taylor_serie_exp(1.106875, 3);
    printf("v = %f;\n", v);
    // Salida esperada: v = 2.945480;

    return 0;
}
