#include "cinefiliaHeader.h"

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

