#ifndef CINEFILIAHEADER_H_INCLUDED
#define CINEFILIAHEADER_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define NOMYAPELARGO 60
#define EXITO 0
#define NADA_ANTES_ARR -1
#define VALOR_INC -2
#define PUNT_ANT_ARROB -3
#define PUNTO_DESP_ARR -4
#define DOS_PUNT_SEG -5
#define PUNTO_FINAL -6
#define SIN_ARR -7
#define MAS_UN_ARR -8
#define MAS_DOS_PUNT -9
#define SIN_PUNTO -10
#define SEXONT -11
#define DNI_FUER_RANG -12
#define ERROR_MEMORIA -13
#define FECHA_INVALIDA -14
#define EDAD_MENOR_10 -15
#define FECHA_AFIL_MAL -16
#define CUOTA_FUERA_RANG -17
#define ERROR_TUTOR -18
#define ERROR_VALID -19



#define TAMCUIL 14
#define TAMCAT 10
#define TAMPLAN 10
#define MAIL 30
#define REG 150
///Macros
#define miToLower(c) ( ((c) >= 'A' && (c) <= 'Z') ? ((c) + ('a' - 'A') ): (c) )
#define miToUpper(c) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - ('a' - 'A') ): (c) )
#define esLetra(c) ( ( ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') ) ? 1 : 0 )
#define esAlpha(c) ( ( (( (c) >= 'A' && (c) <= 'Z')) || ((c) >= 'a' && (c) <= 'z') ) ||  ( (c) >= 0 && (c) <= 9 ) ? 1 : 0 )
typedef struct{
    int d, m, a;
}t_fecha;

typedef struct{
    long dni;
    char cuil[TAMCUIL];
    char nya[NOMYAPELARGO];
    t_fecha fnac;
    char sexo;
    char categoria[TAMCAT];
    t_fecha fechaAfiliacion;
    t_fecha ultimaCuota;
    char estado;
    char plan[TAMPLAN];
    char emailTutor[MAIL];
}t_miembros;


typedef struct{
    t_miembros *vec;
    int cantidad;
    int capacidad;
}t_vector;

char *crearCuil(long dni, char sexo);
int calcularDigito(int *tipo, long dni);
int validarCorreo(char* correo);
void mostrarErrorCorreo(int codigo);
int validarDni(long dni);
int validarSexo(char sexo);
char *normalizarNombre(char *nya);
int validarUltimaCuota(t_fecha *fCuota, t_fecha *fAfil, t_fecha *fProc);
int validarFechaAfil(t_fecha *fAfil, t_fecha *fNac, t_fecha *fProc);
int validarFechaNac(t_fecha *fNac, t_fecha *fProc);
int valEmailTut(char *emailT, t_fecha *fNac, t_fecha *fProc);

//TDA VECTOR
void vector_crear(t_vector *v);
int vector_insertar(t_vector *v, t_miembros nuevo);
void vector_destruir(t_vector *v);


//FECHAS
bool esFechaValida(const t_fecha *f);
int cantDiaMes(int m, int a);
bool esBisiesto(int a);
void mostrarFecha(const t_fecha *f);
int compararFecha(const t_fecha*, const t_fecha*);
t_fecha restarDiasAFecha(const t_fecha*f, int dias);
int diferenciaEntreFechas(t_fecha*, t_fecha*);
int diaDeLaSemana(t_fecha*);

#endif // CINEFILIAHEADER_H_INCLUDED
