#include "cinefiliaHeader.h"

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

    *ptr = (*tipo)/10; //ahi me queda la decena, que va a se 2 o 3

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
            return 9;
            *tipo = 23;
        }

        else
        {
            *tipo = 23;
            return 4;
        }

    }

    return (11 - resto);
}

char *crearCuil(long dni, char sexo)
{
    //con los guiones el cuil queda en 14 caracteres
    char *cuil = (char*)malloc(14*sizeof(char));
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

int validarCorreo(char *correo)
{
    int cantArrob = 0, cantPto = 0;
    char *posArrob = NULL;
    char *posPunt = NULL;
    if(*correo == '@')
        return NADA_ANTES_ARR;
    while(*correo != '\0')
    {
        if(!isalnum(*correo))
            if(*correo != '.' && *correo != '@')
                return VALOR_INC;
        if(*correo == '@')
        {
            cantArrob++;
            posArrob = correo;
        }
        if(*correo == '.' && cantArrob == 0)
            return PUNT_ANT_ARROB;
        if(*correo == '.' && cantArrob == 1)
        {
            if(correo == (posArrob + 1))
                return PUNTO_DESP_ARR;
            if(cantPto >= 1 && correo == (posPunt + 1))
                return DOS_PUNT_SEG;
            cantPto++;
            posPunt = correo;
        }
        correo++;
    }
    if(*(correo-1) == '.')
        return PUNTO_FINAL;
    if(cantArrob == 0)
        return SIN_ARR;
    if(cantArrob > 1)
        return MAS_UN_ARR;
    if(cantPto > 2)
        return MAS_DOS_PUNT;
    if(cantPto == 0)
        return SIN_PUNTO;
    return EXITO;
}

int validarDni(long dni)
{
    return dni >= 1000000 && dni<= 1000000000 ? EXITO : DNI_FUER_RANG;
}

int validarSexo(char sexo)
{
    return sexo == 'M' || sexo == 'F' ? EXITO : SEXONT;
}

void mostrarErrorCorreo(int codigo)
{
    switch(codigo)
    {
        case EXITO:           puts("Correo valido.");break;
        case NADA_ANTES_ARR:  puts("Error: no hay caracteres antes del @.");break;
        case VALOR_INC:       puts("Error: caracter invalido en el correo.");break;
        case PUNT_ANT_ARROB:  puts("Error: hay un punto antes del @.");break;
        case PUNTO_DESP_ARR:  puts("Error: hay un punto justo despues del @.");break;
        case DOS_PUNT_SEG:    puts("Error: hay dos puntos seguidos en el dominio.");break;
        case PUNTO_FINAL:     puts("Error: el correo termina con un punto.");break;
        case SIN_ARR:         puts("Error: el correo no tiene @.");break;
        case MAS_UN_ARR:      puts("Error: el correo tiene mas de un @.");break;
        case MAS_DOS_PUNT:    puts("Error: hay mas de dos puntos en el dominio.");break;
        case SIN_PUNTO:       puts("Error: no hay punto en el dominio.");break;
    }
}
