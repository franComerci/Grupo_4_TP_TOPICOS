#include <stdio.h>
#include <stdlib.h>
#include "cinefiliaHeader.h"
int main()
{
    char *resultado = crearCuil(12345678, 'E');

    printf("cuil: %s\n", resultado);

    char correojoder[] = " a@a r.a";

    int corrval = validarCorreo(correojoder);

    mostrarErrorCorreo(corrval);

    free(resultado);

    char nya[60] = "    galleTa   pePE , raul";
    printf("Resultado: '%s'\n", normalizarNombre(nya));

    FILE *arch;

    if(arch = fopen("miembros.dat","r") == NULL)
    {
        puts("Error al abrir el archivo");
        getch();
        exit(1);
    }

    leerArchivo(arch);

    fclose (arch);

    return 0;
}
