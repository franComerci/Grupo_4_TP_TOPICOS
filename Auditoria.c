#include "cinefiliaHeader.h"

static const char *nombres_op[]  = {"Alta", "Baja", "Modificacion", "Consulta"};
static const char *nombres_ent[] = {"Miembros", "Pelis", "Alquileres"};

// ================================================================
//  audi_registrar
//  Incrementa la celda [op][ent] de la matriz segun el resultado.
//  resultado == EXITO (0) o OK (1) => exito; cualquier otro => error.
// ================================================================
void audi_registrar(t_auditoria *aud, e_operacion op, e_entidad ent, int resultado)
{
    if (resultado == EXITO)
        aud->celdas[op][ent].exitos++;
    else
        aud->celdas[op][ent].errores++;
}

void audi_mostrar(const t_auditoria *aud)
{
    printf("\n========== AUDITORIA (matriz operacion x entidad) ==========\n");
    printf("%-15s %-14s %8s %8s\n", "Operacion", "Entidad", "Exitos", "Errores");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < N_OPERACIONES; i++)
    {
        for (int j = 0; j < N_ENTIDADES; j++)
        {
            // Solo muestra filas con al menos un movimiento
            unsigned ex  = aud->celdas[i][j].exitos;
            unsigned err = aud->celdas[i][j].errores;
            if (ex > 0 || err > 0)
                printf("%-15s %-14s %8u %8u\n",
                       nombres_op[i], nombres_ent[j], ex, err);
        }
    }
    printf("=============================================================\n");
}


void audi_guardar(const t_auditoria *aud, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
    {
        printf("Error: no se pudo crear %s\n", path);
        return;
    }

    // Encabezado
    fprintf(f, "Operacion;Entidad;Exitos;Errores\n");

    // Recorre la matriz fila por fila (operacion) y columna por columna (entidad)
    for (int i = 0; i < N_OPERACIONES; i++)
    {
        for (int j = 0; j < N_ENTIDADES; j++)
        {
            fprintf(f, "%s;%s;%u;%u\n",
                    nombres_op[i],
                    nombres_ent[j],
                    aud->celdas[i][j].exitos,
                    aud->celdas[i][j].errores);
        }
    }

    fclose(f);
    printf("Auditoria guardada en '%s'.\n", path);

    // Tambien la muestra por pantalla al cerrar
    audi_mostrar(aud);
}


void errores_guardar(const t_indice *indErrores, const char *path)
{
    if (indice_vacio(indErrores) == OK)
    {
        printf("Sin errores de carga que guardar.\n");
        return;
    }

    FILE *f = fopen(path, "w");
    if (!f)
    {
        printf("Error: no se pudo crear %s\n", path);
        return;
    }

    fprintf(f, "DNI,TipoError,Fecha,EmailTutor\n");

    t_error_carga *vec = (t_error_carga *)indErrores->vindice;
    for (unsigned i = 0; i < indErrores->cantidad_elementos_actual; i++)
    {
        fprintf(f, "%ld,%s,%02d/%02d/%04d,%s\n",
                (vec + i)->dni,
                (vec + i)->tipoError,
                (vec + i)->fecha.d, (vec + i)->fecha.m, (vec + i)->fecha.a,
                (vec + i)->email);
    }

    fclose(f);
    printf("Errores de carga (%u) guardados en '%s'.\n",
           indErrores->cantidad_elementos_actual, path);
}
