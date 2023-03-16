#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Uso: %s <valor numerico>\n", argv[0]);
        exit(1);
    }
    printf("El valor es %d\n", atoi(argv[1]));
    return 0;
}