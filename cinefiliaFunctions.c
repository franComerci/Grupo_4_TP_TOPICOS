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
