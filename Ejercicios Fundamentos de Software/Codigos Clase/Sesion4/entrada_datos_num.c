#include <stdio.h>
#define T 8

int main()
{
    int n1, n2;
    char b[T];

    printf("Introduzca dos enteros y una cadena");
    scanf("%d %d %s", &n1, &n2, b);

    printf("n1 = %d\n", n1);
    printf("n2 = %d\n", n2);
    printf("s = %s\n", b);

    return 0;
}
