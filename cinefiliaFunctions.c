#include "cinefiliaHeader.h"

/*
Alta títulos/miembro: se obtendrán los datos del teclado, ingresando primero el DNI verificando
que no exista en el índice. Una vez ingresados todos los datos del miembro, realizar la validación
y consistencia de estos (ídem proceso de generación del archivo). Insertar en forma ordenada en
el índice.Si se detectan errores, se ignora todo lo ingresado.
*/

int comparar_dni(const void *dniA, const void *dniB)
{
    return (((const t_miembros *)dniA)->dni - ((const t_miembros *)dniB)->dni);
}

///PUNTO A (ALTA DE MIEMBROS) TERMINAR !!!
int AltaMiembros(const char *arch, t_indice *vec_indices, t_vector *v, size_t cantelem, size_t tam, t_fecha fProc)
{
    FILE *maestro = fopen(arch,"r+b");
    if(maestro == NULL)
    {
        puts("Error al abrir archivo");
        //getch();
        return ERROR_ARCHIVO;
    }
    t_miembros aux;
    printf("Ingrese el DNI del miembro a dar de Alta: ");
    scanf("%ld",&aux.dni);
    int band = indice_buscar(vec_indices, &aux.dni, vec_indices->cantidad_elementos_actual, sizeof(t_reg_indice), comparar_dni);

    if(validarDni(aux.dni) != EXITO)
    {
        fclose(maestro);
        return DNI_FUER_RANG;
    }

    if(band != NO_EXISTE)
        return ERROR_DNI_DUP;

    fflush(stdin);
    printf("Ingrese el nombre y apellido del miembro a dar de Alta: ");
    LeerTexto(aux.nya, STRING);
    normalizarNombre(aux.nya);
    printf("Para el miembro a dar de alta: ");
    ingresarFecha(&aux.fnac);
    band = validarFechaNac(&aux.fnac, &fProc);
    if(band != EXITO)
    {
        fclose(maestro);
        return EDAD_MENOR_10;
    }
    validarCat(&fProc,&aux);

    if(strcmp(aux.categoria,"MAYOR"))
    {
        printf("Al ser menor, el miembro debe registrar un email de un tutor/a: ");
        LeerTexto(aux.emailTutor,TAM_TUTOR);
        band = validarCorreo(aux.emailTutor);
        if(band!=EXITO)
        {
            fclose(maestro);
            return band;
        }
    }

    fflush(stdin);
    printf("Ingrese el sexo del miembro a dar de alta (M/F/O): ");
    scanf("%c",&aux.sexo);
    band = validarSexo(aux.sexo);

    if(band!=EXITO)
    {
        fclose(maestro);
        return SEXONT;
    }
    char *cuilgen = crearCuil(aux.dni,aux.sexo);
    if(cuilgen == NULL)
    {
        fclose(maestro);
        return ERROR_MEMORIA;
    }
    strcpy(aux.cuil,cuilgen);
    free(cuilgen);

    //FALTA VALIDAR FECHA DE PROCESO
    //FALTA VALIDAR ULTIMA CUOTA
    aux.estado = 'A';
    fflush(stdin);
    printf("Ingrese el plan al que pertenece el miembro a dar de alta: ");
    LeerTexto(aux.plan,TAM_PLAN);
    band = validarPlan(aux.plan);

    if(band!=EXITO)
    {
        fclose(maestro);
        return ERROR_PLAN;
    }

    fwrite(&aux,sizeof(t_miembros),1,maestro);

    t_reg_indice nuevoind;
    nuevoind.dni = aux.dni;
    nuevoind.nro_reg = v->cantidad;
    indice_insertar(vec_indices,&aux,sizeof(t_reg_indice),comparar_dni);
    vector_insertar(v,aux);
    fclose(maestro);
}

void LeerTexto (char texto[], int largo)
{
    int i=0;
	fgets(texto, largo, stdin);
	while (texto[i]!='\0')
	{
	    if (texto[i]=='\n')
		    texto[i]='\0';
		else
		    i++;
	}
}

///PUNTO B (ALTA DE UN TITULO) FALTANATE!!!

///PUNTO C (BAJA DE MIEMBROS) PROBAR !!!
int BajaMiembros(t_indice *indice, const char  *nombreArch, long dniBorrar)
{
    t_miembros miembroAux ={0};
    miembroAux.dni = dniBorrar;
    int pos = indice_buscar(indice, &miembroAux, indice->cantidad_elementos_actual, sizeof(t_miembros),comparar_dni);
    if(pos == NO_EXISTE)
        return ERROR;
    FILE *arch = fopen(nombreArch, "r+b");
    if(!arch)
        return ERROR_ARCHIVO;
    t_miembros aux;
    fseek(arch, pos * sizeof(t_miembros), 0);
    if(fread(&aux, sizeof(t_miembros), 1, arch))
    {
        if(aux.dni == dniBorrar)
        {
            aux.estado = 'B';
            fseek(arch, pos * sizeof(t_miembros), 0);//me posiciono de vuelta
            fwrite(&aux, sizeof(t_miembros), 1, arch); //actualizo
        }
    }
    indice_eliminar(indice, &miembroAux, sizeof(t_miembros), comparar_dni);
    fclose(arch);
    return OK;
}

///PUNTO D (BAJA DE UN TITULO) FALTANATE!!!

/// PUNTO E (MODIFICACION DE UN MIEMBRO) FALTANATE!!!

/*int ModificarMiembro(t_indice indice, long dniModif, char const * name)
{
    t_miembros miembro = {0};
    miembro.dni = dniModif;
    int pos = indice_buscar(indice, &miembro, indice->cantidad_elementos_actual, sizeof(t_miembros), comparar_dni);
    if(pos == NO_EXISTE)
        return ERROR;
    t_miembros aux = (t_miembros)indice->vindice;
    t_miembros miembroPos = (aux + pos);//(aux + pos) tiene el dni que hay que modificar

    int opcion, flag = 0; //la flag es para confirmar el cambio y actualizar el archivo
    do
    {
        printf("MODIFICANDO MIEMBRO %s \n", miembroPos.dni)
        printf("1. Modificar Email tutor %s\n", miembroPos.emailTutor );
        printf("2. Modificar plan %s\n", miembroPos.plan);
        printf("3. Guardar \n");
        printf("4. Cancelar \n");
        scanf("%d", &opcion);
        switch(opcion)
        {
            case 1:
            {
                char mailNuevo[MAIL];
                printf("Ingresar mail \n");
                LeerTexto(mailNuevo, MAIL);
                int resul = valEmailTut(mailNuevo, &miembro.fnac, &FECHA_PROCESO);
                if( resul == EXITO)
                {
                    strcpy(miembroPos.emailTutor, mailNuevo);
                }
                else
                    mostrarErrorCorreo(resul);
                break;
            }
            case 2:
            {
                char nuevoPlan[TAMPLAN];
                puts("Ingresar plan \n");
                LeerTexto(nuevoPlan, TAMPLAN);
                if(validarPlan(nuevoPlan) == EXITO)
                    strcpy(miembroPos.plan, nuevoPlan);
                else
                    puts("Plan no valido \n");

                break;
            }
            case 3:
            {
                flag = 1;
                break;
            }
            case 4:
            {
                puts("Cancelado\n");
                return OK;
                break;
            }
            default:
                puts("Opcion invalida \n");
        }
    }while(opcion != 3 && opcion != 4);
    if(flag == 1)
    {
        (aux + pos) = miembroPos;
        FILEarch = fopen(name, "r+b");
        if(arch)
        {
            fseek(arch, pos *sizeof(t_miembros), 0);
            fwrite(&miembroPos, sizeof(t_miembros), 1, arch);
            fclose(arch);
        }
        else
            return ERROR_ARCHIVO;
    }
    return OK;
}*/


/// PUNTO F (MODIFIACION DE UN TITULO) FALTANATE!!!



/// PUNTO G (MOSTRAR INFORMACION DE UN MIEMBRO) FALTANATE!!!



/// PUNTO H (ALQUILER DE UN TITULO) FALTANATE!!!



///PUNTO I (LISTADO DE MIEMBROS ORDENADOS POR DNI) FALTANATE!!!



/// PUNTO J (LISTADO DE MIEMBROS POR PLAN) FALTANATE!!!


