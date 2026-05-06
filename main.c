#include <stdio.h>
#include <stdlib.h>
#include "cinefiliaHeader.h"
int main()
{
    char *resultado = crearCuil(12345678, 'E');

    printf("cuil: %s\n", resultado);

    free(resultado);
    return 0;
}
