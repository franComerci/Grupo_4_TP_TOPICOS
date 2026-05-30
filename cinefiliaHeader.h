#ifndef CINEFILIAHEADER_H_INCLUDED
#define CINEFILIAHEADER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

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

//INDICE
#define CANTIDAD_ELEMENTOS 100
#define INCREMENTO 1.3
#define OK 1
#define ERROR 0
#define NO_EXISTE -1
#define NO_LLENO -2

//ARCHIVOS
//#define PATH_ARCH_MIEMBROS "miembros.dat"

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

typedef struct
{
    unsigned nro_reg;
    long dni;
}t_reg_indice;

typedef struct
{
    void *vindice;
    unsigned cantidad_elementos_actual;
    unsigned cantidad_elementos_maxima;
}t_indice;



/// ------------------------------PROTOTIPOS------------------------------------

///NORMALIZAR
//Miembros
char *crearCuil(long dni, char sexo);
int calcularDigito(int *tipo, long dni);
char *normalizarNombre(char *nya);
char *normalizarNomPel(char *gen_o_tit);
//Peliculas
char *normalizarNomPel(char *gen_o_tit);
int comparar_dni(const void *dniA, const void *dniB);

///GESTION DE ARCHIVOS
void leerArchivo(FILE *archivo, t_vector *vecMiembros, t_fecha fechaProceso);
void trozado(char *linea, t_miembros *m);
int procesarMiembro(char *registro, t_miembros *miembro, t_fecha fechaProceso);
void mostrarArchivoMiembros(const char *pathArch);
int ModificarMiembro(t_indice indice, long dniModif, const char *PATH_ARCH_MIEMBROS);

///INDICE
/**************************************************************************
Descripción: toma memoria para 100 elementos e inicializa la estructura vacía.
Parámetros: indice: TDA índice.
 nmemb: cantidad de elementos del índice.
 tamanyo: el espacio en bytes ocupado por cada elemento.
Retorno: n/a.
Observaciones:
**************************************************************************/
int indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo);
/**************************************************************************
Descripción: redimensiona el tamaño del índice.
Parámetros: indice: TDA índice.
 nmemb: cantidad de elementos del índice.
 tamanyo: el espacio en bytes ocupado por cada elemento.
Retorno: n/a.
Observaciones: Debe proporcionar el nmemb incrementado en un 30%
**************************************************************************/
int indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo);
/**************************************************************************
Descripción: inserta en orden según la clave.
Parámetros: indice: TDA índice.
 registro: el nuevo elemento a insertar en el índice.
 tamanyo: el espacio en bytes ocupado por el elemento a insertar.
 cmp: función de comparación provista.
Retorno: OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: Si el array está lleno, toma un 30 % más de memoria.
**************************************************************************/
int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *));
/**************************************************************************
Descripción: elimina el registro del índice.
Parámetros: indice: TDA índice.
 registro: el elemento a eliminar.
 tamanyo: el espacio en bytes ocupado por el elemento a insertar.
 cmp: función de comparación provista.
Retorno: OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: -
**************************************************************************/
int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int
(*cmp)(const void *, const void *));
/**************************************************************************
Descripción: si la clave existe deja el registro en registro.
Parámetros: indice: TDA índice.
 registro: el elemento a buscar.
 nmemb: cantidad de elementos del índice.
 tamanyo: espacio en bytes ocupado por el elemento a insertar.
 cmp: función de comparación provista.
Retorno: NO_EXISTE si no existe o si existe, la posición ocupada dentro
del array.
Observaciones: -
**************************************************************************/
int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb,
size_t tamanyo, int (*cmp)(const void *, const void *));
/**************************************************************************
Descripción: determina si el índice contiene 0 (cero) elementos.
Parámetros: indice: TDA índice.
Retorno: OK si está vacío, cualquier otro valor si no lo está.
Observaciones: -
**************************************************************************/
int indice_vacio(const t_indice *indice);
/**************************************************************************
descripción: determina si el índice contiene el tamaño máximo posible.
Parámetros: indice: TDA índice.
Retorno: OK si está lleno, cualquier otro valor si no lo está.
Observaciones: -
**************************************************************************/
int indice_lleno(const t_indice *indice);
/**************************************************************************
Descripción: deja el índice vacío.
Parámetros: indice: TDA indice.
Retorno: No posee.
Observaciones: -
**************************************************************************/
void indice_vaciar(t_indice* indice);
/**************************************************************************
Descripción: Carga el array desde un archivo ordenado.
Parámetros: path: la ruta al archivo binario.
 indice: TDA índice.
 vreg_ind: vector de elementos dentro del índice.
 tamanyo: el espacio en bytes ocupado por el elemento a insertar.
 cmp: función de comparación provista.
Retorno: OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: -
**************************************************************************/
int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
tamanyo, int (*cmp)(const void *, const void *));

//MENU
void MostrarMenu();
void EjecutarMenu();

//TDA_FECHA
void ingresarFecha(t_fecha *f);
bool esFechaValida(const t_fecha *f);
int cantDiaMes(int m, int a);
bool esBisiesto(int a);
void mostrarFecha(const t_fecha *f);
int compararFecha(const t_fecha*, const t_fecha*);
t_fecha restarDiasAFecha(const t_fecha*f, int dias);
int diferenciaEntreFechas(t_fecha*, t_fecha*);
int diaDeLaSemana(t_fecha*);

//TDA_VECTOR
void vector_crear(t_vector *v);
int vector_insertar(t_vector *v, t_miembros nuevo);
void vector_destruir(t_vector *v);

///-----------------VALIDACIONES---------------
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
