#include <stdio.h>
#include <stdlib.h>
#include "cinefiliaHeader.h"
#include "gestionArchivos.h"
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


    t_vector vec;
    vector_crear(&vec);
    t_fecha hoy = {18, 5, 2026};

    FILE *arch = fopen("miembros.dat","r+b");
    if(arch == NULL)
    {
        puts("error");
        return 1;
    }
    leerArchivo(arch, &vec, hoy);
    fclose (arch);

    puts("fin de lectura \n");

    printf("cant de registros = %d\n", vec.cantidad);

    for(int i = 0; i < vec.cantidad; i++)
    {
        printf("dni: %ld \n", vec.vec[i].dni);
        printf("nombre: %s \n", vec.vec[i].nya);
        printf("cuil: %s \n", vec.vec[i].cuil);
        printf("categoria: %s \n", vec.vec[i].categoria);
        printf("Sexo: %c \n", vec.vec[i].sexo);
        printf("Estado: %c \n", vec.vec[i].estado);
        printf("plan: %s \n", vec.vec[i].plan);
        printf("categoria: %s \n", vec.vec[i].categoria);

        printf("\n--------------------------------\n");
    }

    vector_destruir(&vec);
    return 0;
}
