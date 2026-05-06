#ifndef CINEFILIAHEADER_H_INCLUDED
#define CINEFILIAHEADER_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct{
    int d, m, a;
}t_fecha;

char *crearCuil(long dni, char sexo);
int calcularDigito(int *tipo, long dni);
void normalizar(char *nombre, char *apellido);
#endif // CINEFILIAHEADER_H_INCLUDED
