#include "MenuDeOperaciones.h"

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
}
