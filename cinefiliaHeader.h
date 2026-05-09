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

char *crearCuil(long dni, char sexo);
int calcularDigito(int *tipo, long dni);
void normalizar(char *nomyape);
int validarCorreo(char* correo);
void mostrarErrorCorreo(int codigo);
int validarDni(long dni);
int validarSexo(char sexo);
char *normalizarNombre(char *nya);
void leerArchivo(FILE* archivo);
void trozado(char linea, t_miembros m)
#endif // CINEFILIAHEADER_H_INCLUDED
