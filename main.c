#include <stdio.h>
#include <stdlib.h>
#include "cinefiliaHeader.h"
int main()
{
    char *resultado = crearCuil(12345678, 'E');

    printf("cuil: %s\n", resultado);

    char correojoder[] = "a@ar.a.";

    int corrval = validarCorreo(correojoder);

    mostrarErrorCorreo(corrval);

    free(resultado);
    return 0;
}
