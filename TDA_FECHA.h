#ifndef TDA_FECHA_H_INCLUDED
#define TDA_FECHA_H_INCLUDED
#include "cinefiliaHeader.h"

//FECHAS
bool esFechaValida(const t_fecha *f);
int cantDiaMes(int m, int a);
bool esBisiesto(int a);
void mostrarFecha(const t_fecha *f);
int compararFecha(const t_fecha*, const t_fecha*);
t_fecha restarDiasAFecha(const t_fecha*f, int dias);
int diferenciaEntreFechas(t_fecha*, t_fecha*);
int diaDeLaSemana(t_fecha*);

#endif // TDA_FECHA_H_INCLUDED
