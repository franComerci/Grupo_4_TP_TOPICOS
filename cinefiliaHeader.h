#ifndef CINEFILIAHEADER_H_INCLUDED
#define CINEFILIAHEADER_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "FuncionesNormalizar.h"
#include "gestionArchivos.h"
#include "Indices.h"
#include "MenuDeOperaciones.h"
#include "TDA_FECHA.h"
#include "TDA_VECTOR.h"


///DEFINE
#define STRING 60
#define TAM_GENERO 20
#define TAM_PLAN 10
#define TAM_TUTOR 30
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
#define ERROR_CATEGORIA -20
#define ERROR_STOCK -21
#define ERROR_ARCHIVO -22
#define ERROR_DNI_DUP -23
#define ERROR_PLAN -24
#define TAMCUIL 14
#define TAMCAT 10
#define TAMPLAN 10
#define MAIL 30
#define REG 150

///MACROS
#define miToLower(c) ( ((c) >= 'A' && (c) <= 'Z') ? ((c) + ('a' - 'A') ): (c) )
#define miToUpper(c) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - ('a' - 'A') ): (c) )
#define esLetra(c) ( ( ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') ) ? 1 : 0 )
#define esAlpha(c) ( ( (( (c) >= 'A' && (c) <= 'Z')) || ((c) >= 'a' && (c) <= 'z') ) ||  ( (c) >= 0 && (c) <= 9 ) ? 1 : 0 )

///STRUCTS
typedef struct
{
    int d, m, a;
}t_fecha;

typedef struct
{
    long dni;
    char cuil[TAMCUIL];
    char nya[STRING];
    t_fecha fnac;
    char sexo;
    char categoria[TAMCAT];
    t_fecha fechaAfiliacion;
    t_fecha ultimaCuota;
    char estado;
    char plan[TAMPLAN];
    char emailTutor[MAIL];
}t_miembros;

typedef struct
{
    t_miembros *vec;
    int cantidad;
    int capacidad;
}t_vector;

typedef struct
{
    int idPeli;
    char titulo[STRING];
    char genero[TAM_GENERO];
    int stock;
}t_pelis;

/// PROTOTIPOS
int comparar_dni(const void *dniA, const void *dniB);

///PUNTO A (ALTA DE MIEMBROS) TERMINAR !!!
int AltaMiembros(const char *arch, t_indice *vec_indices, t_vector *v, size_t cantelem, size_t tam, t_fecha fProc);
void LeerTexto (char texto[], int largo);

///PUNTO B (ALTA DE UN TITULO) FALTANATE!!!


///PUNTO C (BAJA DE MIEMBROS) PROBAR !!!
int BajaMiembros(t_indice *indice, const char  *nombreArch, long dniBorrar);

///PUNTO D (BAJA DE UN TITULO) FALTANATE!!!


/// PUNTO E (MODIFICACION DE UN MIEMBRO) FALTANATE!!!


/// PUNTO F (MODIFIACION DE UN TITULO) FALTANATE!!!


/// PUNTO G (MOSTRAR INFORMACION DE UN MIEMBRO) FALTANATE!!!


/// PUNTO H (ALQUILER DE UN TITULO) FALTANATE!!!


///PUNTO I (LISTADO DE MIEMBROS ORDENADOS POR DNI) FALTANATE!!!



/// PUNTO J (LISTADO DE MIEMBROS POR PLAN) FALTANATE!!!


#endif // CINEFILIAHEADER_H_INCLUDED
