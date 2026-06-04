#include "cinefiliaHeader.h"

// ============================================================
//  FUNCIONES DE COMPARACION
// ============================================================
int comparar_dni(const void *a, const void *b)
{
    long dniA = ((const t_miembros *)a)->dni;
    long dniB = ((const t_miembros *)b)->dni;
    if (dniA < dniB) return -1;
    if (dniA > dniB) return  1;
    return 0;
}

int comparar_id_peli(const void *a, const void *b)
{
    int idA = ((t_pelis *)a)->idPeli;
    int idB = ((t_pelis *)b)->idPeli;
    if (idA < idB) return -1;
    if (idA > idB) return  1;
    return 0;
}

// Comparacion por apellido+nombre para el listado J
int comparar_nya(const void *a, const void *b)
{
    return strcmp(((const t_miembros *)a)->nya, ((const t_miembros *)b)->nya);
}

// Comparacion de alquileres por dni+idPeli (clave compuesta)
int comparar_alquiler(const void *a, const void *b)
{
    const t_alquiler *x = (const t_alquiler *)a;
    const t_alquiler *y = (const t_alquiler *)b;
    if (x->dni < y->dni) return -1;
    if (x->dni > y->dni) return  1;
    if (x->idPeli < y->idPeli) return -1;
    if (x->idPeli > y->idPeli) return  1;
    return 0;
}

// ============================================================
//  UTILIDADES
// ============================================================
void LeerTexto(char texto[], int largo)
{
    int i = 0;
    fgets(texto, largo, stdin);
    while (texto[i] != '\0')
    {
        if (texto[i] == '\n')
            texto[i] = '\0';
        else
            i++;
    }
}

void limpiar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// ============================================================
//  PUNTO A - ALTA DE MIEMBRO
// ============================================================
int AltaMiembros(t_indice *indice, t_fecha fProc)
{
    t_miembros aux = {0};

    // DNI
    printf("DNI: ");
    if (scanf("%ld", &aux.dni) != 1)
    {
        limpiar_buffer();
        return ERROR;
    }
    limpiar_buffer();

    if (validarDni(aux.dni) != EXITO)
    {
        printf("DNI fuera de rango. Operacion cancelada.");
        return ERROR_DNI_DUP;
    }
    if (indice_buscar(indice, &aux, indice->cantidad_elementos_actual, sizeof(t_miembros), comparar_dni) != NO_EXISTE)
    {
        printf("El DNI %ld ya existe. Operacion cancelada.\n", aux.dni);
        return ERROR_DUPLICADO;
    }

    // Nombre y apellido
    printf("Apellidos y Nombres (ej: garcia jose): ");
    LeerTexto(aux.nya, STRING);
    normalizarNombre(aux.nya);

    // Fecha de nacimiento
    printf("Fecha de nacimiento (DD/MM/AAAA): ");
    scanf(" %d/%d/%d", &aux.fnac.d, &aux.fnac.m, &aux.fnac.a);
    limpiar_buffer();
    if (validarFechaNac(&aux.fnac, &fProc) != EXITO)
    {
        printf("Fecha de nacimiento invalida o edad menor a 10. Operacion cancelada.");
        return ERROR;
    }

    // Sexo
    printf("Sexo (M/F/O): ");
    scanf(" %c", &aux.sexo);
    limpiar_buffer();
    aux.sexo = miToUpper(aux.sexo);
    if (validarSexo(aux.sexo) != EXITO)
    {
        printf("Sexo invalido. Operacion cancelada.");
        return ERROR;
    }
    aux.fechaAfiliacion = fProc;
    aux.ultimaCuota = fProc;
    // Estado inicial = 'A'
    aux.estado = 'A';

    // Plan (menu)
    printf("Plan:\n  1. BASIC\n  2. PREMIUM\n  3. VIP\n  4. FAMILY\nOpcion: ");
    int opPlan;
    scanf("%d", &opPlan);
    limpiar_buffer();
    switch (opPlan)
    {
    case 1:
        strcpy(aux.plan, "BASIC");
        break;
    case 2:
        strcpy(aux.plan, "PREMIUM");
        break;
    case 3:
        strcpy(aux.plan, "VIP");
        break;
    case 4:
        strcpy(aux.plan, "FAMILY");
        break;
    default:
        printf("Plan invalido. Operacion cancelada.");
        return ERROR_PLAN;
    }

    // Categoria segun edad
    int edad = fProc.a - aux.fnac.a;
    if (fProc.m < aux.fnac.m || (fProc.m == aux.fnac.m && fProc.d < aux.fnac.d))
        edad--;
    if (edad < 18)
        strcpy(aux.categoria, "MENOR");
    else
        strcpy(aux.categoria, "ADULTO");

    // Email tutor (obligatorio si es MENOR)
    printf("Email tutor (dejar vacio si no aplica): ");
    LeerTexto(aux.emailTutor, MAIL);
    if (valEmailTut(aux.emailTutor, &aux.fnac, &fProc) != EXITO)
    {
        printf("Email de tutor invalido o faltante para menor. Operacion cancelada.");
        return ERROR_TUTOR;
    }

    // CUIL
    char *cuil = crearCuil(aux.dni, aux.sexo);
    if (cuil)
    {
        strcpy(aux.cuil, cuil);
        free(cuil);
    }

    if (indice_insertar(indice, &aux, sizeof(t_miembros), comparar_dni) != OK)
    {
        printf("Error de memoria al insertar.");
        return ERROR_MEMORIA;
    }

    printf("Miembro %s (DNI %ld) dado de alta correctamente.\n", aux.nya, aux.dni);
    return EXITO;
}

// ============================================================
//  PUNTO B - ALTA DE TITULO
// ============================================================
int AltaTitulo(t_indice *indice)
{
    t_pelis aux = {0};
    int i;

    // ID autoincremental
    int maxId = 0;
    t_pelis *arr = (t_pelis *)indice->vindice;
    for ( i = 0; i < (int)indice->cantidad_elementos_actual; i++)
        if ((arr + i)->idPeli > maxId) maxId = (arr + i)->idPeli;
    aux.idPeli = maxId + 1;
    printf("ID asignado: %d\n", aux.idPeli);

    // Titulo
    printf("Titulo: ");
    LeerTexto(aux.titulo, STRING);
    normalizarNomPel(aux.titulo);
    if (aux.titulo[0] == '\0')
    {
        printf("Titulo vacio. Operacion cancelada.");
        return ERROR;
    }

    // Genero
    printf("Genero (Accion/Drama/Comedia/Terror): ");
    LeerTexto(aux.genero, TAM_GENERO);
    normalizarNomPel(aux.genero);
    if (validarGenero(aux.genero) != EXITO)
    {
        printf("Genero invalido. Operacion cancelada.");
        return ERROR_GENERO;
    }

    // Stock
    printf("Stock inicial (>= 0): ");
    scanf("%d", &aux.stock);
    limpiar_buffer();
    if (validarStock(aux.stock) != EXITO)
    {
        printf("Stock invalido. Operacion cancelada.");
        return ERROR_STOCK;
    }

    if (indice_insertar(indice, &aux, sizeof(t_pelis), comparar_id_peli) != OK)
    {
        printf("Error de memoria al insertar.");
        return ERROR_MEMORIA;
    }

    printf("Titulo '%s' dado de alta con ID %d.\n", aux.titulo, aux.idPeli);
    return EXITO;
}

// ============================================================
//  PUNTO C - BAJA DE MIEMBRO
// ============================================================
int BajaMiembros(t_indice *indice, long dniBorrar)
{
    t_miembros aux = {0};
    aux.dni = dniBorrar;
    int pos = indice_buscar(indice, &aux, indice->cantidad_elementos_actual, sizeof(t_miembros), comparar_dni);
    if (pos == NO_EXISTE)
    {
        printf("DNI %ld no encontrado.\n", dniBorrar);
        return ERROR;
    }
    t_miembros *p = (t_miembros *)indice->vindice + pos;
    if (p->estado == 'B')
    {
        printf("El miembro %ld ya estaba dado de baja.\n", dniBorrar);
        return ERROR;
    }
    p->estado = 'B';
    indice_eliminar(indice, &aux, sizeof(t_miembros), comparar_dni);
    printf("Miembro %ld dado de baja correctamente.\n", dniBorrar);
    return EXITO;
}

// ============================================================
//  PUNTO D - BAJA DE TITULO
// ============================================================
int BajaTitulo(t_indice *indice, int idBorrar)
{
    t_pelis aux = {0};
    aux.idPeli = idBorrar;

    int pos = indice_buscar(indice, &aux, indice->cantidad_elementos_actual, sizeof(t_pelis), comparar_id_peli);
    if (pos == NO_EXISTE)
    {
        printf("ID %d no encontrado.\n", idBorrar);
        return ERROR;
    }

    indice_eliminar(indice, &aux, sizeof(t_pelis), comparar_id_peli);

    t_pelis *vec = (t_pelis *)indice->vindice;
    for(int i = pos; i < (int)indice->cantidad_elementos_actual; i++)
        (vec + i)->idPeli--;

    printf("Titulo con ID %d dado de baja correctamente.\n", idBorrar);
    return EXITO;
}

// ============================================================
//  PUNTO E - MODIFICACION DE MIEMBRO
// ============================================================
int ModificarMiembro(t_indice *indice, t_fecha fProc)
{
    long dniModif;
    printf("DNI del miembro a modificar: ");
    if (scanf("%ld", &dniModif) != 1)
    {
        limpiar_buffer();
        return ERROR;
    }
    limpiar_buffer();

    t_miembros aux= {0};
    aux.dni = dniModif;
    int pos = indice_buscar(indice, &aux, indice->cantidad_elementos_actual, sizeof(t_miembros), comparar_dni);
    if (pos == NO_EXISTE)
    {
        printf("DNI %ld no encontrado.\n", dniModif);
        return ERROR;
    }

    t_miembros *m = &((t_miembros *)indice->vindice)[pos];
    t_miembros copia = *m;
    int opcion;
    do
    {
        printf("\n--- Modificando: %s (DNI %ld) ---\n", copia.nya, copia.dni);
        printf("1. Modificar Plan        [actual: %s]\n", copia.plan);
        printf("2. Modificar Email Tutor [actual: %s]\n", copia.emailTutor);
        printf("3. Guardar cambios\n");
        printf("4. Cancelar\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        limpiar_buffer();

        switch (opcion)
        {
        case 1:
        {
            char nuevoPlan[TAMPLAN];
            printf("Nuevo plan (BASIC/PREMIUM/VIP/FAMILY): ");
            LeerTexto(nuevoPlan, TAMPLAN);
            // Normalizar a mayusculas para validar

            for (char *p = nuevoPlan; *p; p++)
                *p = miToUpper(*p);
            if (validarPlan(nuevoPlan) == EXITO)
                strcpy(copia.plan, nuevoPlan);
            else
                printf("Plan invalido, no se modifico.");
            break;
        }
        case 2:
        {
            char nuevoMail[MAIL];
            printf("Nuevo email tutor (dejar vacio si no aplica): ");
            LeerTexto(nuevoMail, MAIL);
            if (valEmailTut(nuevoMail, &copia.fnac, &fProc) == EXITO)
                strcpy(copia.emailTutor, nuevoMail);
            else
                printf("Email invalido o faltante para menor, no se modifico.");
            break;
        }
        case 3:
            *m = copia;
            printf("Cambios guardados en memoria.");
            break;
        case 4:
            printf("Modificacion cancelada.");
            break;
        default:
            printf("Opcion invalida.");
        }
    }
    while (opcion != 3 && opcion != 4);

    return (opcion == 3 || opcion == 4) ? EXITO : ERROR;
}

// ============================================================
//  PUNTO F - MODIFICACION DE TITULO
// ============================================================
int ModificarTitulo(t_indice *indice)
{
    int idModif;
    printf("ID del titulo a modificar: ");
    if (scanf("%d", &idModif) != 1)
    {
        limpiar_buffer();
        return ERROR;
    }
    limpiar_buffer();

    t_pelis aux= {0};
    aux.idPeli = idModif;

    int pos = indice_buscar(indice, &aux, indice->cantidad_elementos_actual, sizeof(t_pelis), comparar_id_peli);
    if (pos == NO_EXISTE)
    {
        printf("ID %d no encontrado.\n", idModif);
        return ERROR;
    }

    t_pelis *p = (t_pelis *)indice->vindice + pos;
    t_pelis copia = *p;
    int opcion;
    do
    {
        printf("\n--- Modificando: %s (ID %d) ---\n", copia.titulo, copia.idPeli);
        printf("1. Modificar Titulo  [actual: %s]\n", copia.titulo);
        printf("2. Modificar Genero  [actual: %s]\n", copia.genero);
        printf("3. Modificar Stock   [actual: %d]\n", copia.stock);
        printf("4. Guardar cambios\n");
        printf("5. Cancelar\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        limpiar_buffer();

        switch (opcion)
        {
        case 1:
        {
            char nuevoTit[STRING];
            printf("Nuevo titulo: ");
            LeerTexto(nuevoTit, STRING);
            normalizarNomPel(nuevoTit);
            if (nuevoTit[0] != '\0')
                strcpy(copia.titulo, nuevoTit);
            else
                printf("Titulo vacio, no se modifico.");
            break;
        }
        case 2:
        {
            char nuevoGen[TAM_GENERO];
            printf("Nuevo genero (Accion/Drama/Comedia/Terror): ");
            LeerTexto(nuevoGen, TAM_GENERO);
            normalizarNomPel(nuevoGen);
            if (validarGenero(nuevoGen) == EXITO)
                strcpy(copia.genero, nuevoGen);
            else
                printf("Genero invalido, no se modifico.");
            break;
        }
        case 3:
        {
            int nuevoStock;
            printf("Nuevo stock: ");
            scanf("%d", &nuevoStock);
            limpiar_buffer();
            if (validarStock(nuevoStock) == EXITO)
                copia.stock = nuevoStock;
            else
                printf("Stock invalido, no se modifico.");
            break;
        }
        case 4:
            *p = copia;
            printf("Cambios guardados en memoria.");
            break;
        case 5:
            printf("Modificacion cancelada.");
            break;
        default:
            printf("Opcion invalida.");
        }
    }
    while (opcion != 4 && opcion != 5);

    return (opcion == 4 || opcion == 5) ? EXITO : ERROR;
}

// ============================================================
//  PUNTO G - MOSTRAR INFORMACION DE UN MIEMBRO
// ============================================================
void MostrarInfoMiembro(t_indice *indice)
{
    long dni;
    printf("DNI del miembro a consultar: ");
    if (scanf("%ld", &dni) != 1)
    {
        //fflush(stdin);
        limpiar_buffer();
        return;
    }
    //fflush(stdin);
    limpiar_buffer();

    t_miembros aux = {0};
    aux.dni = dni;

    int pos = indice_buscar(indice, &aux, indice->cantidad_elementos_actual,
                            sizeof(t_miembros), comparar_dni);
    if (pos == NO_EXISTE)
    {
        printf("DNI %ld no encontrado.\n", dni);
        return;
    }

    t_miembros *m = &((t_miembros *)indice->vindice)[pos];
    printf("\n========== DATOS DEL MIEMBRO ==========\n");
    printf("DNI          : %ld\n",  m->dni);
    printf("CUIL         : %s\n",   m->cuil);
    printf("Nombre       : %s\n",   m->nya);
    printf("Nac          : %02d/%02d/%04d\n", m->fnac.d, m->fnac.m, m->fnac.a);
    printf("Sexo         : %c\n",   m->sexo);
    printf("Categoria    : %s\n",   m->categoria);
    printf("Afiliacion   : %02d/%02d/%04d\n", m->fechaAfiliacion.d, m->fechaAfiliacion.m, m->fechaAfiliacion.a);
    printf("Ultima cuota : %02d/%02d/%04d\n", m->ultimaCuota.d, m->ultimaCuota.m, m->ultimaCuota.a);
    printf("Estado       : %c\n",   m->estado);
    printf("Plan         : %s\n",   m->plan);
    printf("Email tutor  : %s\n",   m->emailTutor[0] ? m->emailTutor : "(sin tutor)");
    printf("========================================\n");
}

// ============================================================
//  PUNTO H - ALQUILER DE UN TITULO
// ============================================================
//le mando el archivo de peliculas porque tengo que actualizar el stock

void AlquilerPeli(t_indice *miembro, t_indice *peli, t_indice *alquileres, const char *NombreArchPelis, t_fecha fProc)
{
    t_miembros auxMiembros = {0};
    t_pelis auxPelis = {0};

    printf("Ingresar DNI del miembro: ");
    scanf("%ld", &auxMiembros.dni);
    printf("Ingresar ID del titulo: ");
    scanf("%d", &auxPelis.idPeli);

    limpiar_buffer();

    int posMiembro = indice_buscar(miembro,&auxMiembros, miembro->cantidad_elementos_actual, sizeof(t_miembros), comparar_dni);
    int posPeli = indice_buscar(peli,&auxPelis, peli->cantidad_elementos_actual, sizeof(t_pelis), comparar_id_peli);


    t_miembros *vecMiembros = (t_miembros *)miembro->vindice;
    t_pelis *vecPelis = (t_pelis *)peli->vindice;

    if(posMiembro == NO_EXISTE || (vecMiembros + posMiembro)->estado == 'B' )
    {
        puts("El miembro no existe o esta dado de baja \n");
        return;
    }
    if(posPeli == NO_EXISTE || (vecPelis + posPeli)->stock == 0 )
    {
        puts("El titulo no existe o no tiene stock \n");
        return;
    }

    (vecPelis + posPeli)->stock -= 1; //Actualizo el stock en memoria

    //Actualizo stock en el archivo
    FILE *arch = fopen(NombreArchPelis, "w"); // "w" limpia el archivo para reescribirlo de cero
    if(arch)
    {
        // Escribimos el encabezado oficial
        fprintf(arch, "IDPelicula;Titulo;Genero;Stock\n");

        // Recorremos el vector en RAM y escribimos linea por linea con punto y coma
        for(int i = 0; i < (int)peli->cantidad_elementos_actual; i++)
        {
            fprintf(arch, "%d;%s;%s;%d\n",
                    (vecPelis + i)->idPeli,
                    (vecPelis + i)->titulo,
                    (vecPelis + i)->genero,
                    (vecPelis + i)->stock);
        }
        fclose(arch);
    }

    t_alquiler auxAlquiler = {0};
    auxAlquiler.dni = (vecMiembros + posMiembro)->dni;
    auxAlquiler.idPeli = (vecPelis + posPeli)->idPeli;

    int posAlq = indice_buscar(alquileres,&auxAlquiler, alquileres->cantidad_elementos_actual, sizeof(t_alquiler), comparar_alquiler);

    if(posAlq != NO_EXISTE)
    {
        t_alquiler *vecAlquiler = (t_alquiler*)alquileres->vindice;
        (vecAlquiler + posAlq)->cantAlquileres ++;
        printf("El miembro %ld ya alquilo el titulo %d %d veces \n", auxAlquiler.dni, auxAlquiler.idPeli, (vecAlquiler + posAlq)->cantAlquileres);

    }
    else
    {
        auxAlquiler.cantAlquileres = 1;
        if (indice_insertar(alquileres, &auxAlquiler, sizeof(t_alquiler), comparar_alquiler) == OK)
        {
            printf("Cantidad de alquileres de %ld: %d\n", auxAlquiler.dni, auxAlquiler.cantAlquileres);
        }
        else
            puts("error de memoria al registrar el alquiler");
    }
}

// ============================================================
//  PUNTO I - LISTADO DE MIEMBROS ORDENADOS POR DNI
// ============================================================
void ListadoPorDni(t_indice *indice)
{
    if (indice_vacio(indice) == OK)
    {
        puts("No hay miembros registrados.");
        return;
    }

    t_miembros *arr = (t_miembros *)indice->vindice;
    int i;

    printf("\n========== LISTADO DE MIEMBROS (por DNI) ==========\n");
    printf("%-12s %-14s %-30s %-10s %-6s\n",
           "DNI", "CUIL", "Nombre", "Plan", "Estado");
    printf("------------------------------------------------------------\n");
    t_miembros *p = arr;
    for (i = 0; i < (int)indice->cantidad_elementos_actual; i++, p++)
    {
        printf("%-12ld %-14s %-30s %-10s %-6c\n",
               p->dni, p->cuil, p->nya, p->plan, p->estado);
    }
    printf("Total: %u miembro/s\n", indice->cantidad_elementos_actual);
}

// ============================================================
//  PUNTO J - LISTADO DE MIEMBROS POR PLAN
// ============================================================
void ListadoPorPlan(t_indice *indice)
{
    if (indice_vacio(indice) == OK)
    {
        printf("No hay miembros registrados.");
        return;
    }

    unsigned n = indice->cantidad_elementos_actual;
    t_miembros *arr = (t_miembros *)indice->vindice;

    // Copiar el arreglo y ordenar por nya (para mostrar ordenado por apellido)
    t_miembros *copia = (t_miembros *)malloc(n * sizeof(t_miembros));
    if (!copia)
    {
        printf("Error de memoria.");
        return;
    }
    memcpy(copia, arr, n * sizeof(t_miembros));

    // Insertion sort por nya
    unsigned i, j;
    for (i = 1; i < n; i++)
    {
        t_miembros tmp = *(copia + i);
        j = i;
        while (j > 0 && comparar_nya(copia + j - 1, &tmp) > 0)
        {
            *(copia + j) = *(copia + j - 1);
            j--;
        }
        *(copia + j) = tmp;
    }
    const char *planes[] = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    int nPlanes = 4;

    printf("\n========== LISTADO POR PLAN ==========\n");
    printf("%-30s", "Plan / Indice");
    int p;
    for (p = 0; p < nPlanes; p++)
        printf("%-12s", planes[p]);
    printf("\n");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        printf("%-30s", copia[i].nya);
        for (p = 0; p < nPlanes; p++)
        {
            if (strcmp(copia[i].plan, planes[p]) == 0)
                printf("%-12ld", copia[i].dni);
            else
                printf("%-12d", 0);
        }
        printf("\n");
    }

    free(copia);
}
