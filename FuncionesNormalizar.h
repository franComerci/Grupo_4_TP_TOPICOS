#ifndef PUNTO1_H_INCLUDED
#define PUNTO1_H_INCLUDED
#include "cinefiliaHeader.h"

//Miembros
char *crearCuil(long dni, char sexo);
int calcularDigito(int *tipo, long dni);
char *normalizarNombre(char *nya);
char *normalizarNomPel(char *gen_o_tit);

//Peliculas
char *normalizarNomPel(char *gen_o_tit);


#endif // PUNTO1_H_INCLUDED
