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



//TDA vector

void vector_crear(t_vector *v)//inicializo el vec y las variables
{
    v->vec = NULL;
    v->cantidad = 0;
    v->capacidad = 0;
}

int vector_insertar(t_vector *v, t_miembros nuevo) //manejo como entran los nuevos miembros
{
    if(v->cantidad >= v->capacidad)
    {
        int nuevaCap =(v->capacidad == 0)? 10: v->capacidad * 2; // primero le asigno 10 si es 0, si no lo duplico
        t_miembros *aux= (t_miembros *)realloc(v->vec, nuevaCap * sizeof(t_miembros)); // calculo la nueva capacidad si es que se lleno el vec

        if(aux == NULL)
            return ERROR_MEMORIA;

        v->vec = aux;
        v->capacidad = nuevaCap;
    }

    v->vec[v->cantidad] = nuevo; // lo pongo en cantidad suponiendo que siempre se inserta al final
    v->cantidad++;

    return EXITO;
}

//fechas
void vector_destruir(t_vector *v)
{
    if(v->vec != NULL)
    {
        free(v->vec);
        v->vec = NULL;
    }

    v->cantidad = 0;
    v->capacidad = 0;
}

//validaciones de fechas para la estructura
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


//funciones de fechas


void ingresarFecha(t_fecha *f)
{
    puts("Ingrese una fecha con formato DD / MM / AAAA:");
    scanf("%02d / %02d / %04d", &f->d, &f->m, &f->a);
    while (!esFechaValida(f))
    {
        puts("Fecha invalida. Recuerde que el formato es DD / MM / AAAA");
        scanf("%02d / %02d / %04d", &f->d, &f->m, &f->a);
    }
}

bool esFechaValida(const t_fecha *f)
{
    if (f->a > 1600 && f->m >= 1 && f->m <= 12 && f->d >= 1 && f->d <= cantDiaMes(f->m, f->a))
    {
        return true;
    }
    return false;
}

int cantDiaMes(int m, int a)
{
    int dias_por_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && esBisiesto(a))
    {
        return 29;
    }
    return *(dias_por_mes+m);
}

bool esBisiesto(int a)
{
    return ((a%4 == 0 && a%100 != 0) || a%400 == 0);
}

t_fecha sumarDiasAFecha(const t_fecha *f, int dias)
{
    int cdm;
    t_fecha f2 = *f;
    f2.d += dias;
    while (f2.d > (cdm = cantDiaMes(f2.m, f2.a)))
    {
        f2.d -= cdm;
        f2.m++;
        if (f2.m > 12)
        {
            f2.m = 1;
            f2.a++;
        }
    }
    return f2;
}

void mostrarFecha(const t_fecha *f)
{
    printf("%02d / %02d / %4d", f->d, f->m, f->a);
}

int compararFecha(const t_fecha *f1, const t_fecha *f2)
{
    return (f1->a == f2->a && f1->d == f2->d && f1->m == f2->m);
}

t_fecha restarDiasAFecha(const t_fecha *f, int dias)
{
    t_fecha f2 = *f;
    f2.d -= dias;
    while (f2.d < 1)
    {
        f2.m--;
        if (f2.m < 1)
        {
            f2.m = 12;
            f2.a--;
        }
        f2.d += cantDiaMes(f2.m, f2.a);
    }
    return f2;
}

int diferenciaEntreFechas(t_fecha *f1, t_fecha *f2)
{
    t_fecha f = *f1;
    int cantDias = 0;

    if (compararFecha(f1,f2))
    {
        return 0;
    }

    if (f1->a > f2->a || (f1->a == f2->a && f1->m > f2->m) || (f1->a == f2->a && f1->m == f2->m && f1->d > f2->d))
    {
        while(!compararFecha(f2, &f))
        {
            f = restarDiasAFecha(&f, 1);
            cantDias--;
        }
    }
    else
    {
        while(!compararFecha(f2, &f))
        {
            f = sumarDiasAFecha(&f, 1);
            cantDias++;
        }
    }
    return cantDias;
}

int diaDeLaSemana(t_fecha *f)
{
    t_fecha referencia = {7, 1, 1990}; // Fue domingo
    return diferenciaEntreFechas(&referencia, f) % 7; // Domingo = 0, Lunes = 1, etc.
}

int valEmailTut(char *emailT, t_fecha *fNac, t_fecha *fProc)
{
    int edad = fProc->a - fNac->a;

    if(fProc->m < fNac->m || (fProc->m == fNac->m && fProc->d < fNac->d)) // por si todavía no cumplió años
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

int validarCategoria(char *cat)
{
   return (strcasecmp(cat, "BASIC")== 0 || strcasecmp(cat, "PREMIUM")== 0 || strcasecmp(cat, "VIP")== 0 || strcasecmp(cat, "FAMILY")== 0)? EXITO : ERROR_CATEGORIA;
}

void leerArchivo (FILE *arch, t_vector *vect, t_fecha hoy)
{
    t_miembros nuevomiemb;


    while(fread(&nuevomiemb,sizeof(t_miembros),1,arch))
    {
        int flag = EXITO;

        flag = (flag == EXITO)? validarDni(nuevomiemb.dni) : flag;
        flag = (flag == EXITO)? validarSexo(nuevomiemb.sexo) : flag;
        flag = (flag == EXITO)? validarFechaNac(&nuevomiemb.fnac, &hoy) : flag;
        flag = (flag == EXITO)? validarFechaAfil(&nuevomiemb.fechaAfiliacion, &nuevomiemb.fnac, &hoy) : flag;
        flag = (flag == EXITO)? valEmailTut(nuevomiemb.emailTutor, &nuevomiemb.fnac, &nuevomiemb.fechaAfiliacion) : flag;
        flag = (flag == EXITO)? validarCategoria(nuevomiemb.categoria) : flag;

        if(flag == EXITO)
        {
            char *cuil = crearCuil(nuevomiemb.dni, nuevomiemb.sexo);
            strcpy(nuevomiemb.cuil, cuil);
            free(cuil);
            normalizarNombre(nuevomiemb.nya);

            flag = vector_insertar(vect,nuevomiemb);
        }
        else
        {
            //FUNCION PARA AUDITAR ERRORES
        }
    }
}
