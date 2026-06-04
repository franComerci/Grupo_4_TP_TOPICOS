#include "cinefiliaHeader.h"

int main()
{
    printf("Arrancando...\n");
    fflush(stdout);

    t_fecha fProc = obtenerFechaProceso();

    // 2. Indices
    t_indice indMiembros, indPelis, indAlquileres, indErrores;

    // 2a. Auditoria de operaciones (matriz en memoria, inicializada en 0)
    t_auditoria auditoria = {0};

    if (indice_crear(&indMiembros, CANTIDAD_ELEMENTOS, sizeof(t_miembros)) != OK)
    {
        printf("Error: no se pudo crear el indice de miembros.\n");
        return ERROR;
    }
    if (indice_crear(&indPelis, CANTIDAD_ELEMENTOS, sizeof(t_pelis)) != OK)
    {
        printf("Error: no se pudo crear el indice de titulos.\n");
        free(indMiembros.vindice);
        return ERROR;
    }
    if (indice_crear(&indAlquileres, CANTIDAD_ELEMENTOS, sizeof(t_alquiler)) != OK)
    {
        printf("Error: no se pudo crear el indice de alquileres.\n");
        free(indMiembros.vindice);
        free(indPelis.vindice);
        return ERROR;
    }
    // Indice de errores de carga CSV (t_error_carga, separado de t_auditoria)
    if (indice_crear(&indErrores, CANTIDAD_ELEMENTOS, sizeof(t_error_carga)) != OK)
    {
        printf("Error: no se pudo crear el indice de errores.\n");
        free(indMiembros.vindice);
        free(indPelis.vindice);
        free(indAlquileres.vindice);
        return ERROR;
    }

    EjecutarMenu(&indMiembros, &indPelis, &indAlquileres,&auditoria, &indErrores,fProc, PATH_MIEMPROS, PATH_TITULOS, PATH_ALQUILERES);


    audi_guardar(&auditoria, "auditoria.csv");


    errores_guardar(&indErrores, "errores_carga.csv");

    // 6. Liberar memoria
    indice_vaciar(&indMiembros);
    indice_vaciar(&indPelis);
    indice_vaciar(&indAlquileres);
    indice_vaciar(&indErrores);

    return 0;
}
