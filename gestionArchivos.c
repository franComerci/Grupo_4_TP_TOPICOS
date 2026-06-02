#include "cinefiliaHeader.h"

t_fecha parsear_fecha(const char *cad)
{
    t_fecha f = {0, 0, 0};
    if (cad == NULL || *cad == '\0')
        return f;
    sscanf(cad, "%d/%d/%d", &f.d, &f.m, &f.a);
    return f;
}

// Formato: DNI;NyA;FechaNac;Sexo;FechaAfil;UltCuota;Estado;Plan;EmailTutor
void trozado(char *linea, t_miembros *m)
{
    char *act = strchr(linea, '\n');
    if (act) *act = '\0';
    act = strchr(linea, '\r');
    if (act) *act = '\0';

    // EmailTutor
    act = strrchr(linea, ';');
    strcpy(m->emailTutor, act + 1);
    *act = '\0';

    // Plan
    act = strrchr(linea, ';');
    strcpy(m->plan, act + 1);
    *act = '\0';

    // Estado
    act = strrchr(linea, ';');
    m->estado = *(act + 1);
    *act = '\0';

    // Ultima cuota
    act = strrchr(linea, ';');
    m->ultimaCuota = parsear_fecha(act + 1);
    *act = '\0';

    // Fecha afiliacion
    act = strrchr(linea, ';');
    m->fechaAfiliacion = parsear_fecha(act + 1);
    *act = '\0';

    // Sexo
    act = strrchr(linea, ';');
    m->sexo = *(act + 1);
    *act = '\0';

    // Fecha nacimiento
    act = strrchr(linea, ';');
    m->fnac = parsear_fecha(act + 1);
    *act = '\0';

    // Nombre y apellido
    act = strrchr(linea, ';');
    strcpy(m->nya, act + 1);
    *act = '\0';

    // DNI
    m->dni = strtol(linea, NULL, 10);

    // cuil y categoria se calculan despues
}

// Trozado de una linea CSV de peliculas
// Formato: IDPelicula;Titulo;Genero;Stock
void trozado_peli(char *linea, t_pelis *p)
{
    char *act = strchr(linea, '\n');
    if (act) *act = '\0';
    act = strchr(linea, '\r');
    if (act) *act = '\0';

    // Stock
    act = strrchr(linea, ';');
    p->stock = atoi(act + 1);
    *act = '\0';

    // Genero
    act = strrchr(linea, ';');
    strcpy(p->genero, act + 1);
    *act = '\0';

    // Titulo
    act = strrchr(linea, ';');
    strcpy(p->titulo, act + 1);
    *act = '\0';

    // ID
    p->idPeli = atoi(linea);
}

int procesarMiembro(char *registro, t_miembros *miembro, t_fecha fechaProceso)
{
    trozado(registro, miembro);
    normalizarNombre(miembro->nya);

    char *cuil = crearCuil(miembro->dni, miembro->sexo);
    if (cuil != NULL)
    {
        strcpy(miembro->cuil, cuil);
        free(cuil);
    }

    if (validarDni(miembro->dni)                                                         != EXITO)
        return ERROR_VALID;
    if (validarSexo(miembro->sexo)                                                       != EXITO)
        return ERROR_VALID;
    if (validarFechaNac(&miembro->fnac, &fechaProceso)                                   != EXITO)
        return ERROR_VALID;
    if (validarFechaAfil(&miembro->fechaAfiliacion, &miembro->fnac, &fechaProceso)       != EXITO)
        return ERROR_VALID;
    if (validarUltimaCuota(&miembro->ultimaCuota, &miembro->fechaAfiliacion, &fechaProceso) != EXITO)
        return ERROR_VALID;
    if (valEmailTut(miembro->emailTutor, &miembro->fnac, &fechaProceso)                  != EXITO)
        return ERROR_VALID;
    if(validarPlan(miembro->plan)!= EXITO)
        return ERROR_VALID;


    // Calcular categoria
    int edad = fechaProceso.a - miembro->fnac.a;
    if (fechaProceso.m < miembro->fnac.m ||
            (fechaProceso.m == miembro->fnac.m && fechaProceso.d < miembro->fnac.d))
        edad--;
    if (edad < 18)
        strcpy(miembro->categoria, "MENOR");
    else
        strcpy(miembro->categoria, "ADULTO");

    return EXITO;
}

int procesarPelicula(char *registro, t_pelis *peli)
{
    trozado_peli(registro, peli);
    normalizarNomPel(peli->titulo);
    normalizarNomPel(peli->genero);

    if (peli->idPeli <= 0)            return ERROR_VALID;
    if (validarGenero(peli->genero)  != EXITO) return ERROR_VALID;
    if (validarStock(peli->stock)    != EXITO) return ERROR_VALID;

    return EXITO;
}

// Carga los dos CSV en sus respectivos indices
void cargarDatos(t_indice *indMiembros, t_indice *indPelis, t_fecha fProc,const char *pathMiembros, const char *pathPelis)
{
    char linea[REG];
    int cargados, errores;

    // --- Miembros ---
    FILE *fMiembros = fopen(pathMiembros, "r");
    if (!fMiembros)
    {
        printf("Advertencia: no se pudo abrir %s\n", pathMiembros);
    }
    else
    {
        cargados = 0;
        errores  = 0;
        fgets(linea, REG, fMiembros); // saltar header
        while (fgets(linea, REG, fMiembros))
        {
            t_miembros m;
            memset(&m, 0, sizeof(t_miembros));/// ARREGLAR
            if (procesarMiembro(linea, &m, fProc) == EXITO)
            {
                if (indice_insertar(indMiembros, &m, sizeof(t_miembros), comparar_dni) == OK)
                    cargados++;
                else
                    errores++;
            }
            else
                errores++;
        }
        fclose(fMiembros);
        printf("Miembros cargados: %d  |  Errores: %d\n", cargados, errores);
    }

    // --- Peliculas ---
    FILE *fPelis = fopen(pathPelis, "r");
    if (!fPelis)
    {
        printf("Advertencia: no se pudo abrir %s\n", pathPelis);
    }
    else
    {
        cargados = 0;
        errores  = 0;
        fgets(linea, REG, fPelis); // saltar header
        while (fgets(linea, REG, fPelis))
        {
            t_pelis p;
            memset(&p, 0, sizeof(t_pelis));
            if (procesarPelicula(linea, &p) == EXITO)
            {
                if (indice_insertar(indPelis, &p, sizeof(t_pelis), comparar_id_peli) == OK)
                    cargados++;
                else
                    errores++;
            }
            else
                errores++;
        }
        fclose(fPelis);
        printf("Titulos cargados: %d  |  Errores: %d\n", cargados, errores);
    }
}

void MostrarArchivos(t_indice *indMiembros, t_indice *indPelis)
{
    t_miembros *arrM = (t_miembros *)indMiembros->vindice;
    t_pelis    *arrP = (t_pelis    *)indPelis->vindice;

    printf("\n============== MIEMBROS (%u) ==============\n",
           indMiembros->cantidad_elementos_actual);

    printf("%-10s %-14s %-30s %-6s %-11s %-11s %-8s %-11s %-10s %-10s %-30s\n",
           "DNI", "CUIL", "Nombre", "Sexo", "FechaNac", "FechaAfil", "Categoria", "UltimaCuota", "Estado", "Plan", "EmailTutor");

    for (int i = 0; i < (int)indMiembros->cantidad_elementos_actual; i++)
    {
        printf("%-10ld %-14s %-30s %-6c %02d/%02d/%04d  %02d/%02d/%04d  %-8s %02d/%02d/%04d  %-9c %-10s %-30s\n",
               (arrM+i)->dni,
               (arrM+i)->cuil,
               (arrM+i)->nya,
               (arrM+i)->sexo,
               (arrM+i)->fnac.d,            (arrM+i)->fnac.m,            (arrM+i)->fnac.a,
               (arrM+i)->fechaAfiliacion.d,  (arrM+i)->fechaAfiliacion.m, (arrM+i)->fechaAfiliacion.a,
               (arrM+i)->categoria,
               (arrM+i)->ultimaCuota.d,      (arrM+i)->ultimaCuota.m,     (arrM+i)->ultimaCuota.a,
               (arrM+i)->estado,
               (arrM+i)->plan,
               (arrM+i)->emailTutor);
    }
    printf("\n============== TITULOS (%u) ==============\n",
           indPelis->cantidad_elementos_actual);
    printf("%-5s %-35s %-12s %-5s\n","ID","Titulo","Genero","Stock");

    for (int i = 0; i < (int)indPelis->cantidad_elementos_actual; i++)
    {
        printf("%-5d %-35s %-12s %-5d\n",
               (arrP+i)->idPeli, (arrP+i)->titulo, (arrP+i)->genero, (arrP+i)->stock);
    }
}
