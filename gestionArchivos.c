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
/*
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
*/
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


int procesarMiembro(char *registro, t_miembros *miembro, t_indice *indAuditoria, t_fecha fechaProceso)
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
    {
        t_auditoria error;
        strcpy(error.tipoError, "DNI");
        error.dni = miembro->dni;
        error.fecha = fechaProceso;
        strcpy(error.email,miembro->emailTutor);
        indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
        return ERROR_VALID;
    }
    if (validarSexo(miembro->sexo)                                                       != EXITO)
    {
        t_auditoria error;
        strcpy(error.tipoError, "SEXO");
        error.dni = miembro->dni;
        error.fecha = fechaProceso;
        strcpy(error.email,miembro->emailTutor);
        indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
        return ERROR_VALID;
    }
    if (validarFechaNac(&miembro->fnac, &fechaProceso)                                   != EXITO)
        {
            t_auditoria error;
            strcpy(error.tipoError, "F_NAC");
            error.dni = miembro->dni;
            error.fecha = fechaProceso;
            strcpy(error.email,miembro->emailTutor);
            indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
            return ERROR_VALID;
        }
    if (validarFechaAfil(&miembro->fechaAfiliacion, &miembro->fnac, &fechaProceso)       != EXITO)
        {
            t_auditoria error;
            strcpy(error.tipoError, "F_AFIL");
            error.dni = miembro->dni;
            error.fecha = fechaProceso;
            strcpy(error.email,miembro->emailTutor);
            indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
            return ERROR_VALID;
        }
    if (validarUltimaCuota(&miembro->ultimaCuota, &miembro->fechaAfiliacion, &fechaProceso) != EXITO)
        {
            t_auditoria error;
            strcpy(error.tipoError, "U_CUOTA");
            error.dni = miembro->dni;
            error.fecha = fechaProceso;
            strcpy(error.email,miembro->emailTutor);
            indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
            return ERROR_VALID;
        }
    if (valEmailTut(miembro->emailTutor, &miembro->fnac, &fechaProceso)                  != EXITO)
        {
            t_auditoria error;
            strcpy(error.tipoError, "EMAIL");
            error.dni = miembro->dni;
            error.fecha = fechaProceso;
            strcpy(error.email,miembro->emailTutor);
            indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
            return ERROR_VALID;
        }
    if(validarPlan(miembro->plan)!= EXITO)
        {
            t_auditoria error;
            strcpy(error.tipoError, "PLAN");
            error.dni = miembro->dni;
            error.fecha = fechaProceso;
            strcpy(error.email,miembro->emailTutor);
            indice_insertar(indAuditoria, &error, sizeof(t_auditoria),comparar_auditoria);
            return ERROR_VALID;
        }

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

// Carga los dos CSV en sus respectivos indices
void cargarDatos(t_indice *indMiembros, t_indice *indPelis, t_indice *indAuditoria, t_fecha fProc,const char *pathMiembros, const char *pathPelis)
{
    char linea[REG];
    int cargados, errores;

    //puedo cargar el archivo del dia o el maestro
    char miembrosFProc[100];
    char titulosFProc[100];
    sprintf(miembrosFProc, "miembros_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);
    sprintf(titulosFProc, "titulos_%d_%d_%d.csv", fProc.d, fProc.m,fProc.a);

    // --- Miembros ---
    FILE *fMiembros = fopen(miembrosFProc, "r");
    if (fMiembros)
    {
        printf("Se detecto archivo diario, abriendo %s\n", miembrosFProc);
    }
    else
    {
        printf("No se detecto archivo diario, abriendo %s \n", pathMiembros);
        fMiembros = fopen(pathMiembros, "r");
    }

    if(fMiembros)
    {
        cargados = 0;
        errores  = 0;
        fgets(linea, REG, fMiembros); // saltar header
        while (fgets(linea, REG, fMiembros))
        {
            t_miembros m = {0};

            if (procesarMiembro(linea, &m, indAuditoria, fProc) == EXITO)
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

    FILE *fPelis = fopen(titulosFProc, "r");
    if (fPelis)
    {
        printf("Se detecto archivo diario, abriendo %s\n", titulosFProc);
    }
    else
    {
        printf("No se detecto archivo diario, abriendo %s \n", pathPelis);
        fPelis = fopen(pathPelis, "r");
    }


    if (!fPelis)
    {
        printf("Advertencia: no se pudo abrir %s\n", pathPelis);
    }


    if(fPelis)
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


void guardarDatos(t_indice *miembros, t_indice *titulos, t_indice *alquileres, t_fecha fProc)
{
    char nombreMiembros[100];
    char nombreTitulos[100];
    char nombreAlquiler[100];

    sprintf(nombreMiembros, "miembros_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);
    sprintf(nombreTitulos, "titulos_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);
    sprintf(nombreAlquiler, "alquiler_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);

    FILE *pfMiembros = fopen(nombreMiembros, "w");
    if(pfMiembros)
    {
        fprintf(pfMiembros,"DNI;NYA;NACIMIENTO;SEXO;AFILIACION;ULT_CUOTA;ESTADO;PLAN;EMAIL_TUTOR\n");
        t_miembros *vecM = (t_miembros*)miembros->vindice;
        for(int i = 0; i < (int)miembros->cantidad_elementos_actual; i++)
        {
            fprintf(pfMiembros, "%ld;%s;%02d/%02d/%04d;%c;%02d/%02d/%04d;%02d/%02d/%04d;%c;%s;%s\n",
                    (vecM + i)->dni, (vecM + i)->nya,
                    (vecM + i)->fnac.d, (vecM + i)->fnac.m, (vecM + i)->fnac.a,
                    (vecM + i)->sexo,
                    (vecM + i)->fechaAfiliacion.d, (vecM + i)->fechaAfiliacion.m, (vecM + i)->fechaAfiliacion.a,
                    (vecM + i)->ultimaCuota.d, (vecM + i)->ultimaCuota.m, (vecM + i)->ultimaCuota.a,
                    (vecM + i)->estado, (vecM + i)->plan, (vecM + i)->emailTutor
                    );

        }
        fclose(pfMiembros);
        printf("Archivo %s actualizado\n", nombreMiembros);
    }


    FILE *pfTitulos = fopen(nombreTitulos, "w");
    if(pfTitulos)
    {
        fprintf(pfMiembros,"ID_PELI;TITULO;GENERO;STOCK\n");
        t_pelis *vecP = (t_pelis*)titulos->vindice;
        for(int i = 0; i < (int)titulos->cantidad_elementos_actual; i++)
        {
            fprintf(pfTitulos, "%d;%s;%s;%d\n",
                    (vecP + i)->idPeli, (vecP + i)->titulo,
                    (vecP + i)->genero, (vecP + i)->stock
                    );

        }
        fclose(pfTitulos);
        printf("Archivo %s actualizado\n", nombreTitulos);
    }



    FILE *pfAlquiler = fopen(nombreAlquiler, "w");
    if(pfAlquiler)
    {
        fprintf(pfAlquiler,"DNI;ID_PELI;CANTIDAD\n");
        t_alquiler *vecA = (t_alquiler*)alquileres->vindice;
        for(int i = 0; i < (int)alquileres->cantidad_elementos_actual; i++)
        {
            fprintf(pfAlquiler, "%ld; %d; %d\n", (vecA+ i)->dni, (vecA + i)->idPeli, (vecA + i)->cantAlquileres);

        }
        fclose(pfAlquiler);
        printf("Archivo %s actualizado\n", nombreAlquiler);
    }
}
