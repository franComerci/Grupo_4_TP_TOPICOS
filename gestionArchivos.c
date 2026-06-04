#include "cinefiliaHeader.h"

int comparar_error_carga(const void *a, const void *b)
{
    long dniA = ((const t_error_carga *)a)->dni;
    long dniB = ((const t_error_carga *)b)->dni;
    if (dniA < dniB) return -1;
    if (dniA > dniB) return  1;
    return 0;
}

static void registrar_error_carga(t_indice *indErrores, long dni, const char *tipo, t_fecha fecha, const char *email)
{
    t_error_carga err = {0};
    err.dni   = dni;
    err.fecha = fecha;
    strcpy(err.tipoError, tipo);
    strcpy(err.email, email);
    indice_insertar(indErrores, &err, sizeof(t_error_carga), comparar_error_carga);
}


t_fecha parsear_fecha(const char *cad)
{
    t_fecha f = {0, 0, 0};
    if (cad == NULL || *cad == '\0')
        return f;
    sscanf(cad, "%d/%d/%d", &f.d, &f.m, &f.a);
    return f;
}

void trozado(char *linea, t_miembros *m)
{
    char *act = strchr(linea, '\n');
    if (act) *act = '\0';
    act = strchr(linea, '\r');
    if (act) *act = '\0';

    // EmailTutor (ultimo campo)
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

    // DNI (lo que queda al inicio)
    m->dni = strtol(linea, NULL, 10);

    // cuil y categoria se calculan en procesarMiembro()
}

void trozado_peli(char *linea, t_pelis *p)
{
    char *act = strchr(linea, '\n');
    if (act) *act = '\0';
    act = strchr(linea, '\r');
    if (act) *act = '\0';

    // Stock (ultimo campo)
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

int procesarMiembro(char *registro, t_miembros *miembro, t_indice *indErrores, t_fecha fechaProceso)
{

    trozado(registro, miembro);
    normalizarNombre(miembro->nya);

    char *cuil = crearCuil(miembro->dni, miembro->sexo);
    if (cuil != NULL)
    {
        strcpy(miembro->cuil, cuil);
        free(cuil);
    }

    if (validarDni(miembro->dni) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "DNI", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }
    if (validarSexo(miembro->sexo) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "SEXO", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }
    if (validarFechaNac(&miembro->fnac, &fechaProceso) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "F_NAC", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }
    if (validarFechaAfil(&miembro->fechaAfiliacion,
                         &miembro->fnac, &fechaProceso) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "F_AFIL", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }
    if (validarUltimaCuota(&miembro->ultimaCuota,
                           &miembro->fechaAfiliacion, &fechaProceso) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "U_CUOTA", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }
    if (valEmailTut(miembro->emailTutor,
                    &miembro->fnac, &fechaProceso) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "EMAIL", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }
    if (validarPlan(miembro->plan) != EXITO)
    {
        registrar_error_carga(indErrores, miembro->dni,
                              "PLAN", fechaProceso, miembro->emailTutor);
        return ERROR_VALID;
    }

    // --- Calcular categoria segun edad
    int edad = fechaProceso.a - miembro->fnac.a;
    if (fechaProceso.m < miembro->fnac.m ||
            (fechaProceso.m == miembro->fnac.m &&
             fechaProceso.d < miembro->fnac.d))
        edad--;

    strcpy(miembro->categoria, edad < 18 ? "MENOR" : "ADULTO");

    return EXITO;
}

int procesarPelicula(char *registro, t_pelis *peli)
{
    trozado_peli(registro, peli);
    normalizarNomPel(peli->titulo);
    normalizarNomPel(peli->genero);

    if (peli->idPeli <= 0)                return ERROR_VALID;
    if (validarGenero(peli->genero) != EXITO) return ERROR_VALID;
    if (validarStock(peli->stock)   != EXITO) return ERROR_VALID;

    return EXITO;
}

void cargarDatos(t_indice *indMiembros, t_indice *indPelis, t_indice *indErrores,t_indice *indAlquileres, t_fecha fProc, const char *pathMiembros, const char *pathPelis)
{

    char miembrosDia[100], pelisDia[100];
    char linea[REG];


    sprintf(miembrosDia, "miembros_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);
    sprintf(pelisDia, "titulos_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);

    int cargados, errores;

    // --- Miembros ---
    FILE *fMiembros = fopen(miembrosDia, "r");
    if (fMiembros)
    {
        printf("Se encontro un archivo con los miembros de este dia: '%s'.\n", miembrosDia);
    }
    else
    {
        printf("No hay archivo de este dia, se usa el archivo maestro: %s \n", pathMiembros);
        fMiembros = fopen(pathMiembros, "r");
    }

    if(fMiembros)
    {
        cargados = 0;
        errores  = 0;
        fgets(linea, REG, fMiembros);

        while (fgets(linea, REG, fMiembros))
        {
            t_miembros m = {0};

            if (procesarMiembro(linea, &m, indErrores, fProc) == EXITO)
            {

                if (indice_buscar(indMiembros, &m,
                                  indMiembros->cantidad_elementos_actual,
                                  sizeof(t_miembros), comparar_dni) != NO_EXISTE)
                {
                    registrar_error_carga(indErrores, m.dni,
                                          "DNI_DUP", fProc, m.emailTutor);
                    errores++;
                }
                else if (indice_insertar(indMiembros, &m,
                                         sizeof(t_miembros), comparar_dni) == OK)
                {
                    cargados++;
                }
                else
                {
                    registrar_error_carga(indErrores, m.dni,
                                          "MEM", fProc, m.emailTutor);
                    errores++;
                }
            }
            else
                errores++;
        }
        fclose(fMiembros);
        printf("Miembros cargados: %d  |  Errores: %d\n", cargados, errores);
    }

    // --- Peliculas ---
    FILE *fPelis = fopen(pelisDia, "r");
    if (fPelis)
    {
        printf("Se encontro un archivo con los titulos de este dia: '%s'.\n", pelisDia);
    }
    else
    {
        printf("No hay archivo de este dia, se usa el archivo maestro: %s \n", pathPelis);
        fPelis = fopen(pathPelis, "r");
    }

    if(fPelis)
    {
        cargados = 0;
        errores  = 0;
        fgets(linea, REG, fPelis);

        while (fgets(linea, REG, fPelis))
        {
            t_pelis p = {0};
            if (procesarPelicula(linea, &p) == EXITO)
            {
                if (indice_insertar(indPelis, &p,
                                    sizeof(t_pelis), comparar_id_peli) == OK)
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

    char alquileresDia[100];

    sprintf(alquileresDia, "alquiler_%d_%d_%d.csv", fProc.d, fProc.m, fProc.a);

    // --- Alquileres ---
    FILE *fAlq = fopen(alquileresDia, "r");
    if (fAlq)
    {
        printf("Se encontro un archivo con los alquileres de este dia: '%s'. Cargando persistencia...\n", alquileresDia);

        cargados = 0;
        errores  = 0;
        fgets(linea, REG, fAlq);

        while (fgets(linea, REG, fAlq))
        {
            t_alquiler alq = {0};


            if (sscanf(linea, "%ld;%d;%d", &alq.dni, &alq.idPeli, &alq.cantAlquileres) == 3)
            {

                if (indice_insertar(indAlquileres, &alq, sizeof(t_alquiler), comparar_alquiler) == OK)
                {
                    cargados++;
                }
                else
                {
                    errores++;
                }
            }
            else
            {
                errores++;
            }
        }
        fclose(fAlq);
        printf("Alquileres recuperados: %d  |  Errores de lectura: %d\n", cargados, errores);
    }
    else
    {
        printf("No hay registros de alquileres previos para el dia de hoy (%02d/%02d/%04d).\n", fProc.d, fProc.m, fProc.a);
    }
}

void MostrarArchivos(t_indice *indMiembros, t_indice *indPelis)
{
    t_miembros *arrM = (t_miembros *)indMiembros->vindice;
    t_pelis    *arrP = (t_pelis    *)indPelis->vindice;

    printf("\n============== MIEMBROS (%u) ==============\n",
           indMiembros->cantidad_elementos_actual);
    printf("%-10s %-14s %-30s %-6s %-11s %-11s %-9s %-11s %-7s %-10s %-30s\n",
           "DNI","CUIL","Nombre","Sexo","FechaNac","FechaAfil",
           "Categoria","UltimaCuota","Estado","Plan","EmailTutor");

    for (int i = 0; i < (int)indMiembros->cantidad_elementos_actual; i++)
    {
        printf("%-10ld %-14s %-30s %-6c %02d/%02d/%04d  %02d/%02d/%04d  "
               "%-8s %02d/%02d/%04d  %-6c %-10s %-30s\n",
               (arrM+i)->dni,
               (arrM+i)->cuil,
               (arrM+i)->nya,
               (arrM+i)->sexo,
               (arrM+i)->fnac.d,           (arrM+i)->fnac.m,           (arrM+i)->fnac.a,
               (arrM+i)->fechaAfiliacion.d, (arrM+i)->fechaAfiliacion.m,(arrM+i)->fechaAfiliacion.a,
               (arrM+i)->categoria,
               (arrM+i)->ultimaCuota.d,    (arrM+i)->ultimaCuota.m,    (arrM+i)->ultimaCuota.a,
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
               (arrP+i)->idPeli,
               (arrP+i)->titulo,
               (arrP+i)->genero,
               (arrP+i)->stock);
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
