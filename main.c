<<<<<<< HEAD
=======

/*
#include <stdio.h>
#include <stdlib.h>
#include "cinefiliaHeader.h"
#include "gestionArchivos.h"
#include "Indices.h"

int main()
{

    char *resultado = crearCuil(12345678, 'E');
    printf("cuil: %s\n", resultado);
    char correojoder[] = " a@a r.a";
    int corrval = validarCorreo(correojoder);
    mostrarErrorCorreo(corrval);
    free(resultado);
    char nya[60] = "    galleTa   pePE , raul";
    printf("Resultado: '%s'\n", normalizarNombre(nya));

    t_vector vec;
    vector_crear(&vec);
    t_fecha hoy = {18, 5, 2026};

    FILE *arch = fopen("miembros.dat","r+b");
    if(arch == NULL)
    {
        puts("error");
        return 1;
    }
    leerArchivo(arch, &vec, hoy);
    fclose (arch);

    puts("fin de lectura \n");

    printf("cant de registros = %d\n", vec.cantidad);

    for(int i = 0; i < vec.cantidad; i++)
    {
        printf("dni: %ld \n", vec.vec[i].dni);
        printf("nombre: %s \n", vec.vec[i].nya);
        printf("cuil: %s \n", vec.vec[i].cuil);
        printf("categoria: %s \n", vec.vec[i].categoria);
        printf("Sexo: %c \n", vec.vec[i].sexo);
        printf("Estado: %c \n", vec.vec[i].estado);
        printf("plan: %s \n", vec.vec[i].plan);
        printf("categoria: %s \n", vec.vec[i].categoria);

        printf("\n--------------------------------\n");
    }


    vector_destruir(&vec);



    return 0;
}
*/

>>>>>>> a98f1e4cfaae65484fd44e121ef105cbc94cf33c
#include "cinefiliaHeader.h"

int main()
{
<<<<<<< HEAD
    mostrarArchivoMiembros("miembros.dat");
    EjecutarMenu();
=======
    EjecutarMenu();
    
>>>>>>> a98f1e4cfaae65484fd44e121ef105cbc94cf33c
    /*
    printf("=== FASE 2: ARRANQUE DEL MOTOR DE BASE DE DATOS ===\n\n");

    // 1. Inicializar el TDA
    t_indice indice_miembros;
    if (indice_crear(&indice_miembros, 100, sizeof(t_miembros)) != OK)
    {
        printf("Error: No se pudo inicializar la memoria del TDA.\n");
        return ERROR_MEMORIA;
    }

    // 2. Carga masiva desde el binario
    t_miembros buffer_lectura;
    printf("[PROCESO] Ejecutando indice_cargar sobre 'miembros.dat'...\n");

    int resultado = indice_cargar("miembros.dat", &indice_miembros, &buffer_lectura, sizeof(t_miembros), comparar_dni);

    // 3. Verificación de resultados
    if (resultado == OK)
    {
        printf("[EXITO] Carga completada en tiempo O(N). Registros en RAM: %d\n\n", indice_miembros.cantidad_elementos_actual);

        t_miembros *arreglo = (t_miembros *)indice_miembros.vindice;

        // Imprimimos el contenido para confirmar que se leyeron los datos correctos y están ordenados
        for (unsigned i = 0; i < indice_miembros.cantidad_elementos_actual; i++)
        {
            printf("Pos %d | DNI: %-8ld | Nombre: %-30s | Categoria: %s\n",
                   i, arreglo[i].dni, arreglo[i].nya, arreglo[i].categoria);
        }
    }
    else
    {
        printf("[ERROR] Fallo la carga del indice.\n");
    }

    // 4. Limpieza
    free(indice_miembros.vindice);

    printf("\n=== SISTEMA CERRADO ===\n");
    return 0;

    */
<<<<<<< HEAD
}
=======
}
>>>>>>> a98f1e4cfaae65484fd44e121ef105cbc94cf33c
