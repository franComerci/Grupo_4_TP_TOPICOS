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

typedef struct{
    int d, m, a;
}t_fecha;

char *crearCuil(long dni, char sexo);
int calcularDigito(int *tipo, long dni);
void normalizar(char *nomyape);
int validarCorreo(char* correo);
void mostrarErrorCorreo(int codigo);
int validarDni(long dni);
int validarSexo(char sexo);
#endif // CINEFILIAHEADER_H_INCLUDED
