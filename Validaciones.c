#include "Validaciones.h"

int validarCorreo(char *correo)
{
    int cantArrob = 0, cantPto = 0;
    char *posArrob = NULL;
    char *posPunt = NULL;
    if(*correo == '@')
        return NADA_ANTES_ARR;
    while(*correo != '\0')
    {
        if(!esAlpha(*correo))
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

int validarDni(long dni)
{
    return dni >= 1000000 && dni<= 1000000000 ? EXITO : DNI_FUER_RANG;
}

int validarSexo(char sexo)
{
    return sexo == 'M' || sexo == 'F' ? EXITO : SEXONT;
}

//Validaciones de fecha
int validarFechaNac(t_fecha *fNac, t_fecha *fProc)
{
    if(!esFechaValida(fNac)) return FECHA_INVALIDA;
    int edad = fProc->a - fNac->a;

    return (edad >= 10) ? EXITO : EDAD_MENOR_10;
}

int validarFechaAfil(t_fecha *fAfil, t_fecha *fNac, t_fecha *fProc)
{
    if(!esFechaValida(fAfil)) return FECHA_INVALIDA;

    int dif = diferenciaEntreFechas(fNac, fProc);

    int difProceso = diferenciaEntreFechas(fAfil, fProc);
    return (dif >= 0 && difProceso >= 0)? EXITO: FECHA_AFIL_MAL;
}

int validarUltimaCuota(t_fecha *fCuota, t_fecha *fAfil, t_fecha *fProc)
{
    if(!esFechaValida(fCuota)) return FECHA_INVALIDA;
    int difAfil = diferenciaEntreFechas(fAfil, fCuota);
    int difProc = diferenciaEntreFechas(fCuota, fProc);

    return (difAfil >= 0 && difProc >= 0)? EXITO : CUOTA_FUERA_RANG;
}

int validarPlan(char *cat)
{
   return (strcasecmp(cat, "BASIC")== 0 || strcasecmp(cat, "PREMIUM")== 0 || strcasecmp(cat, "VIP")== 0 || strcasecmp(cat, "FAMILY")== 0)? EXITO : ERROR_PLAN;
}

int validarGenero(char *genero)
{
    return (strmp(*genero, "Accion") == 0 || strmp(*genero, "Drama")  == 0 || strmp(*genero, "Comedia") == 0 || strmp(*genero, "Terror") == 0) ? EXITO : ERROR_CATEGORIA;
}

int validarStock(int stock)
{
    return stock >= 0 ? EXITO : ERROR_STOCK;
}

int valEmailTut(char *emailT, t_fecha *fNac, t_fecha *fProc)
{
    int edad = fProc->a - fNac->a;

    if(fProc->m < fNac->m || (fProc->m == fNac->m && fProc->d < fNac->d)) // por si todav�a no cumpli� a�os
        edad--;

    if(edad < 18)
    {
        if(*(emailT) == '\0')
            return ERROR_TUTOR;
        else
            return validarCorreo(emailT);
    }

    if(*(emailT)!= '\0') // si es mayor de edad puede tener mail de tutor igual, se verifica que sea correcto
        return validarCorreo(emailT);


    return EXITO;
}

void validarCat(t_fecha *fProc, t_miembros *miembro)
{
    int edad = fProc->a - miembro->fnac.a;
    if(fProc->m < miembro->fnac.m || (fProc->m == miembro->fnac.m && fProc->d < miembro->fnac.d)) // por si todav�a no cumpli� a�os
            edad--;

    if(edad < 18)
        strcpy(miembro->categoria,"MENOR");
    else
        strcpy(miembro->categoria,"MAYOR");
}