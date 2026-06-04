#include "cinefiliaHeader.h"

int main()
{
    // 1. Obtener fecha de proceso
    t_fecha fProc = obtenerFechaProceso();

    // 2. Crear indices
    t_indice indMiembros, indPelis, indAlquileres, indAuditoria;

    if (indice_crear(&indMiembros, CANTIDAD_ELEMENTOS, sizeof(t_miembros)) != OK)
    {
        puts("Error: no se pudo crear el indice de miembros.");
        return ERROR;
    }
    if (indice_crear(&indPelis, CANTIDAD_ELEMENTOS, sizeof(t_pelis)) != OK)
    {
        puts("Error: no se pudo crear el indice de titulos.");
        free(indMiembros.vindice);
        return ERROR;
    }
    if (indice_crear(&indAlquileres, CANTIDAD_ELEMENTOS, sizeof(t_alquiler)) != OK)
    {
        puts("Error: no se pudo crear el indice de alquileres.");
        free(indMiembros.vindice);
        free(indPelis.vindice);
        return ERROR;
    }

    if(indice_crear(&indAuditoria, CANTIDAD_ELEMENTOS, sizeof(t_auditoria)) != OK)
    {
        puts("Error: no se pudo crear la matriz de incidencias.");
        free(indMiembros.vindice);
        free(indPelis.vindice);
        free(indAlquileres.vindice);
    }
    // 3. Ejecutar menu (carga CSV adentro)
    EjecutarMenu(&indMiembros, &indPelis, &indAlquileres, &indAuditoria,fProc, PATH_MIEMPROS, PATH_TITULOS, PATH_ALQUILERES);
    // 4. Liberar memoria
    free(indMiembros.vindice);
    free(indPelis.vindice);
    free(indAlquileres.vindice);

    audi_guardar(&indAuditoria,"Incidencias.csv");
    return 0;
}
