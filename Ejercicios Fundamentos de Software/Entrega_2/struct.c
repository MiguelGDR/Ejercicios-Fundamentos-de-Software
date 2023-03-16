#include <stdio.h>

#define T 5

struct PDU
{
    size_t length;      // Unsigned int ocupa 4 bytes   
    int *payload;       // Puntero int ocupa 4 bytes
};

size_t memoria(struct PDU a[T])
{
    size_t t = 0;
    for (int i = 0; i < T; i++)
    {
        t += sizeof(a[i].length) + (a[i].length * sizeof(int));
    }
    return t;
}

int main()
{
    struct PDU a[T];
    a[0].length = 18;
    int a0[] = {7, 0, 8, 6, 8, 5, 0, 1, 8, 2, 9, 7, 9, 7, 5, 9, 5, 9};
    a[0].payload = a0;

    a[1].length = 14;
    int a1[] = {6, 3, 9, 4, 1, 4, 0, 5, 2, 4, 7, 1, 1, 9};
    a[1].payload = a1;

    a[2].length = 2;
    int a2[] = {9, 9};
    a[2].payload = a2;

    a[3].length = 7;
    int a3[] = {1, 0, 7, 3, 1, 6, 5};
    a[3].payload = a3;

    a[4].length = 19;
    int a4[] = {3, 4, 5, 2, 9, 2, 7, 8, 6, 0, 2, 6, 6, 6, 1, 3, 9, 4, 5};
    a[4].payload = a4;

    unsigned int b = memoria(a);
    printf("%d\n", b);
}