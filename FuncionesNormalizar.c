#include "FuncionesNormalizar.h"


/// PUNTO 1
char *crearCuil(long dni, char sexo)
{
    //con los guiones el cuil queda en 14 caracteres
    char *cuil = (char*)malloc(TAMCUIL*sizeof(char));
//    char cuil[14];
    if(cuil == NULL)
        return NULL;
    int tipo;

    if(sexo == 'M')
        tipo = 20;
    else if(sexo == 'F')
        tipo = 27;
    else
        tipo = 30;

    //le tengo que mandar la dir de tipo porque puede cambiar a 23
    int digitoVerif = calcularDigito(&tipo, dni);

    sprintf(cuil, "%d-%ld-%d", tipo, dni, digitoVerif);

    return cuil;
}

int calcularDigito(int *tipo, long dni)
{
    int nros[] = {5,4,3,2,7,6,5,4,3,2};
    int suma = 0;
    int separado[10];
///para poder multiplicar todo con el vector de nros tengo que desarmar el tipo y el dni
    int *ptr = separado + 9; // arranco desde el final porque voy a usar % para desarmarlo
    long auxDni = dni;
    for (int i = 0; i < 8; i++)
    {
        *ptr = auxDni % 10; //guardo el ultimo nro del dni en el separado[]
        ptr--;
        auxDni /=10; //le saco el ultimo nro al dni
    }
    //termina el for y estoy en separado[1]
    *ptr = (*tipo) % 10;
    ptr--; //separado[0]

    *ptr = (*tipo)/10; //ahi me queda la decena, que va a ser 2 o |

    ptr = separado;
    int *mult = nros;
    for(int i = 0; i < 10; i++)
    {
        suma += (*ptr) * (*mult);
        ptr++;
        mult++;
    }

    int div = suma / 11;
    int resto = suma - (div * 11);

    if(resto == 0)
        return 0;

    if(resto == 1)
    {
        if( *tipo == 20)
        {
            *tipo = 23;
            return 9;
        }

        else
        {
            *tipo = 23;
            return 4;
        }

    }

    return (11 - resto);
}

////PUNTO 2
char *normalizarNombre(char *nya)
{
    if(nya == NULL)
        return NULL;

    char *read = nya;
    char *write = nya;

    while(*read != '\0' && !esLetra(*read)) //le agrego el \0 porque con una cadena vacia se rompe
        read++;

    ///primero voy a sacar todo lo que no sea letra
    while(*read != '\0')
    {
        *write = miToUpper(*read); //primer letra
        write++;
        read++;
        while(*read != '\0' && esLetra(*read))//resto del nombre
        {
            *write = miToLower(*read);
            write++;
            read++;
        }
        while(*read != '\0' && !esLetra(*read)) //no le doy bola a la basura ni a los espacios
            read++;

        if(*read != '\0')
        {
            *write = ' ';
            write++;
        }

    }
    *write = '\0'; //llego al fin, solamente con nombres y apellido, sin basura,
    char *ptr_espacio = nya;

    while(*ptr_espacio != ' ' && *ptr_espacio != '\0')
    {
        ptr_espacio++;
    }

    if(*ptr_espacio == ' ')
    {
        size_t mover = strlen(ptr_espacio) + 1;
        memmove(ptr_espacio + 1, ptr_espacio, mover); // hago el memmove para poner la coma donde va, o sea despues del primer espacio

        *ptr_espacio = ',';
    }

    return nya;
}

char *normalizarNomPel(char *gen_o_tit)
{
    if(gen_o_tit == NULL)
        return NULL;

    char *read = gen_o_tit;
    char *write = gen_o_tit;

    while(*read != '\0' && !esLetra(*read)) //le agrego el \0 porque con una cadena vacia se rompe
        read++;

    ///primero voy a sacar todo lo que no sea letra
    while(*read != '\0')
    {
        *write = miToUpper(*read); //primer letra
        write++;
        read++;
        while(*read != '\0' && esLetra(*read))//resto del nombre
        {
            *write = miToLower(*read);
            write++;
            read++;
        }
        while(*read != '\0' && !esLetra(*read)) //no le doy bola a la basura ni a los espacios
            read++;

        if(*read != '\0')
        {
            *write = ' ';
            write++;
        }

    }
    *write = '\0';
    return gen_o_tit;

}