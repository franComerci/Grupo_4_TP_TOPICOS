#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED
#include "cinefiliaHeader.h"

int validarCorreo(char* correo);
void mostrarErrorCorreo(int codigo);
int valEmailTut(char *emailT, t_fecha *fNac, t_fecha *fProc);
int validarPlan(char *cat);
void LeerTexto (char texto[], int largo);
void validarCat(t_fecha *fProc, t_miembros *miembro);
int validarDni(long dni);
int validarSexo(char sexo);


//Validaciones de fechas
int validarUltimaCuota(t_fecha *fCuota, t_fecha *fAfil, t_fecha *fProc);
int validarFechaAfil(t_fecha *fAfil, t_fecha *fNac, t_fecha *fProc);
int validarFechaNac(t_fecha *fNac, t_fecha *fProc);

//Validar Peliculas
int validarGenero(char *genero);
int validarStock(t_pelis *v);


#endif // VALIDACIONES_H_INCLUDED
