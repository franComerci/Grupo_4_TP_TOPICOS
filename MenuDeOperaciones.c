#include "cinefiliaHeader.h"

void MostrarMenu()
{
    printf("\n=========================================\n"); 
    printf("         CINEFILIA - MENU PRINCIPAL      \n");
    printf("=========================================\n");
    printf("A. Alta de un Miembro\n");
    printf("B. Alta de un Titulo\n");
    printf("C. Baja de un Miembro\n");
    printf("D. Baja de un Titulo\n");
    printf("E. Modificacion de un Miembro\n");
    printf("F. Modificacion de un Titulo\n");
    printf("G. Mostrar Informacion de un Miembro\n");
    printf("H. Alquiler de un Titulo\n");
    printf("I. Listado de Miembros Ordenados por DNI\n");
    printf("J. Listado de Miembros Por Plan\n");
    printf("L. Mostrar todos los registros\n");
    printf("K. Salir\n");
    printf("=========================================\n");
    printf("Opcion: ");
}

void EjecutarMenu(t_indice *indMiembros, t_indice *indPelis, t_indice *alquileres,t_fecha fProc, const char *pathMiembros, const char *pathPelis,const char *pathAlq)
{
    // Cargar datos desde CSV al inicio
    cargarDatos(indMiembros, indPelis, fProc, pathMiembros, pathPelis);// SUBE EL CSV A LOS INDICES

    char opcion;
    do
    {
        MostrarMenu();
        scanf(" %c", &opcion);
        fflush(stdin);
        opcion = miToUpper(opcion);
        switch (opcion)
        {
        case 'A':
            AltaMiembros(indMiembros, fProc);
            break;

        case 'B':
            AltaTitulo(indPelis);
            break;

        case 'C':
        {
            long dni;
            printf("DNI del miembro a dar de baja: ");
            scanf("%ld", &dni);
            fflush(stdin);
            BajaMiembros(indMiembros, dni);
            break;
        }

        case 'D':
        {
            int id;
            printf("ID del titulo a dar de baja: ");
            scanf("%d", &id);
            fflush(stdin);
            BajaTitulo(indPelis, id);
            break;
        }

        case 'E':
            ModificarMiembro(indMiembros, fProc);
            break;

        case 'F':
            ModificarTitulo(indPelis);
            break;

        case 'G':
            MostrarInfoMiembro(indMiembros);
            break;

        case 'H':
            //AlquilarTitulo(indMiembros, indPelis, alquileres); /// FALTA HACER
            AlquilerPeli(indMiembros, indPelis, alquileres, "peliculas.csv",fProc);
            break;

        case 'I':
            ListadoPorDni(indMiembros);
            break;

        case 'J':
            ListadoPorPlan(indMiembros);
            break;

        case 'L':
            MostrarArchivos(indMiembros, indPelis);
            break;

        case 'K':
            puts("\nCerrando programa...");
            // grabado(); GRABANDO EN ARCHIVOS NO REALIZADO HACER
            break;

        default:
            printf("Opcion '%c' invalida. Intente nuevamente.\n", opcion);
            break;
        }
    }
    while (opcion != 'K');
}

t_fecha obtenerFechaProceso()
{
    t_fecha f;
    char opcion;
    printf("\nDesea ingresar la fecha de proceso manualmente? (S/N): ");
    fflush(stdout);
    scanf(" %c", &opcion);
    while(opcion != 'S' && opcion != 's' && opcion !='N' && opcion != 'n' )
    {
        printf("Opcion Invalida. Ingresar Nuevamente: ");
        scanf(" %c", &opcion);
    }
    if (opcion == 'S' || opcion == 's')
    {
        printf("Ingrese la fecha de proceso (DD/MM/AAAA): ");
        limpiar_buffer();
        scanf(" %d/%d/%d", &f.d, &f.m, &f.a);
        while (!esFechaValida(&f))
        {
            printf("Fecha invalida. Reingrese (DD/MM/AAAA): ");
            scanf(" %d/%d/%d", &f.d, &f.m, &f.a);
        }
    }
    else
    {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        f.d = tm_info->tm_mday;
        f.m = tm_info->tm_mon + 1;
        f.a = tm_info->tm_year + 1900;
    }
    printf("Fecha de proceso: %02d/%02d/%04d\n", f.d, f.m, f.a);
    return f;
}
