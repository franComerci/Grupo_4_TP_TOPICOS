#ifndef GESTIONARCHIVOS_H_INCLUDED
#define GESTIONARCHIVOS_H_INCLUDED
#include "cinefiliaHeader.h"
void leerArchivo(FILE *archivo, t_vector *vecMiembros, t_fecha fechaProceso);
void trozado(char *linea, t_miembros *m);
int procesarMiembro(char *registro, t_miembros *miembro, t_fecha fechaProceso);

#endif // GESTIONARCHIVOS_H_INCLUDED
