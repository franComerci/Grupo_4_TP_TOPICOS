<<<<<<< HEAD
#include "cinefiliaHeader.h"
=======
#include "MenuDeOperaciones.h"
>>>>>>> a98f1e4cfaae65484fd44e121ef105cbc94cf33c

void MostrarMenu()
{
    printf("\n=========================================\n");
    printf("           MENU PRINCIPAL\n");
    printf("===========================================\n");
    printf("A. Alta de un Miembro\n");
    printf("B. Alta de un Titulo\n");
    printf("C. Baja de un Miembro\n");
    printf("D. Baja de un Titulo\n");
    printf("E. Modificacion de un Miembro\n");
    printf("F. Modificaion de un Titulo\n");
    printf("G. Mostrar Informacion de un Miembro\n");
    printf("H. Alquiler de un Titulo\n");
    printf("I. Listado de Miembros Ordenados por DNI\n");
    printf("J. Listado de Miembros Por Plan\n");
    printf("K. Salir\n");
    printf("=========================================\n");
}

void EjecutarMenu()
{
<<<<<<< HEAD
    char opcion;
    do
    {
        MostrarMenu();
        printf("Ingrese una opcion: ");
        scanf("%c", &opcion);
        switch(opcion)
        {
            case 'A':
                //AltaMiembros();
            break;
            case 'B':
                //AltaTitulos();
            break;
            case 'C':
                //BajaMiembros();
            break;
            case 'D':
                //BajaTitulos();
            break;
            case 'E':
                //ModifcarMiembro();
            break;
            case 'F':
                //ModificarTitulo();
            break;
            case 'G':
                //MostarInfo();
            break;
            case 'H':
                //Alquilar();
            break;
            case 'I':
                //ListadoPorDni();
            break;
            case 'J':
                //ListarPorPlan();
            break;
            case 'K':
                //Salir();
            break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
            break;
        }
    } while(opcion != 0);
=======
    int opcion;
    do 
    {
        mostrarMenu();
        printf("Ingrese una opcion: ");
        scanf("%c", &opcion);

        switch(opcion)
        {
            case 'A':

            break;
            case 'B':

            break;
            case 'C':

            break;   

            

      
        }
    }
>>>>>>> a98f1e4cfaae65484fd44e121ef105cbc94cf33c
}
