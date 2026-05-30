#include "cinefiliaHeader.h"

t_fecha parsear_fecha(const char *cad)
{
    t_fecha f = {0, 0, 0}; //f.d = 0, f.m = 0, f.a = 0

    if(cad == NULL ||*cad == '\0')
        return f;

    sscanf(cad, "%d/%d/%d", &f.d, &f.m, &f.a);
    return f;
}

void trozado(char *linea, t_miembros *m)
{
    char *act = strchr(linea, '\n');
    if (act)
        *act = '\0';
    //emailTutor
    act = strrchr(linea, ';');
    strcpy(m->emailTutor, act + 1);
    *act = '\0';

    //plan
    act = strrchr(linea, ';');
    strcpy(m->plan, act + 1);
    *act = '\0';

    //estado
    act = strrchr(linea, ';');
    m->estado = *(act + 1);
    *act = '\0';

    // ultima fecha paga
    act = strrchr(linea, ';');
    m->ultimaCuota = parsear_fecha(act + 1);
    *act = '\0';

    // fecha afiliacion
    act = strrchr(linea, ';');
    m->fechaAfiliacion = parsear_fecha(act + 1);
    *act = '\0';

    // sexo
    act = strrchr(linea,';');
    m->sexo = *(act + 1);
    *act = '\0';

    // fecha nacimiento
    act = strrchr(linea, ';');
    m->fnac = parsear_fecha(act + 1);
    *act = '\0';

    //nombre y apellido
    act = strrchr(linea, ';');
    strcpy(m->nya, act + 1);
    *act = '\0';

    //dni
    m->dni = strtol(linea, NULL, 10); //STRTOL = string to long

    //cuil y categoria van a quedar con basura
}

void leerArchivo(FILE *archivo, t_vector *vecMiembros, t_fecha fechaProceso)
{
    t_miembros miembro;
    char registro[REG];
    if(archivo == NULL)
    {
        printf("error con el archivo\n");
         return;
    }
    fgets(registro,sizeof(registro),archivo);

    while (fgets(registro,sizeof(registro),archivo) != NULL)
    {

        if(procesarMiembro(registro, &miembro, fechaProceso) == EXITO)
        {
            if(vector_insertar(vecMiembros, miembro) == ERROR_MEMORIA)
            {
                puts("error de memoria\n");
                return;
            }
        }

    }
}

void mostrarArchivoMiembros(const char *pathArch)
{
    FILE *pf = fopen(pathArch, "rb");
    if (pf == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", pathArch);
        return;
    }
    t_miembros reg;
    printf("\n=========================================================================================================\n");
    printf("%-11s | %-15s | %-25s | %s | %-7s | %c | %-8s | %-30s\n",
           "DNI", "CUIL", "NOMBRE Y APELLIDO", "F.NAC", "CAT", "E", "PLAN", "EMAIL TUTOR");
    printf("=========================================================================================================\n");
    while (fread(&reg, sizeof(t_miembros), 1, pf) == 1)
    {
        printf("%-11ld | %-15s | %-25s | %02d/%02d/%04d | %-7s | %c | %-8s | ",
               reg.dni,
               reg.cuil,
               reg.nya,
               reg.fnac.d, reg.fnac.m, reg.fnac.a,
               reg.categoria,
               reg.estado,
               reg.plan);

        if (strcmp(reg.categoria, "MENOR") == 0)
            printf("%-30s\n", reg.emailTutor);
        else
            printf("%-30s\n", "");
    }
    printf("=========================================================================================================\n\n");
    fclose(pf);
}

int procesarMiembro(char *registro, t_miembros *miembro, t_fecha fechaProceso)
{
    trozado(registro, miembro);
    normalizarNombre(miembro->nya);

    char *cuil = crearCuil(miembro->dni, miembro->sexo);
    if(cuil != NULL)
    {
        strcpy(miembro->cuil, cuil);
    }
    if(validarDni(miembro->dni) != EXITO || validarSexo(miembro->sexo) != EXITO || validarFechaNac(&miembro->fnac, &fechaProceso) != EXITO ||
        validarFechaAfil(&miembro->fechaAfiliacion, &miembro->fnac, &fechaProceso) != EXITO || validarUltimaCuota(&miembro->ultimaCuota, &miembro->fechaAfiliacion, &fechaProceso) != EXITO) // falta agregar las validaciones de fecha
            return ERROR_VALID;


    int edad = fechaProceso.a - miembro->fnac.a;
    if(edad < 18)
        strcpy(miembro->categoria, "MENOR");
    else
        strcpy(miembro->categoria, "ADULTO");

    return EXITO;
}

