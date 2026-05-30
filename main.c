#include "cinefiliaHeader.h"

int main()
{
    mostrarArchivoMiembros("miembros.dat");
    EjecutarMenu();
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
}
