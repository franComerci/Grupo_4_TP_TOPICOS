#ifndef CINEFILIAHEADER_H_INCLUDED
#define CINEFILIAHEADER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

///DEFINE
#define STRING      60
#define TAM_GENERO  20
#define TAM_PLAN    10
#define TAM_TUTOR   50
#define TAMCUIL     14
#define TAMCAT      10
#define TAMPLAN     10
#define MAIL        50
#define REG         200
#define TAM_TITULO  60
#define TAM_NOMBRE  60
#define TAM_TELEFONO 20

// Codigos de retorno
#define EXITO            0
#define NADA_ANTES_ARR  -1
#define VALOR_INC       -2
#define PUNT_ANT_ARROB  -3
#define PUNTO_DESP_ARR  -4
#define DOS_PUNT_SEG    -5
#define PUNTO_FINAL     -6
#define SIN_ARR         -7
#define MAS_UN_ARR      -8
#define MAS_DOS_PUNT    -9
#define SIN_PUNTO      -10
#define SEXONT         -11
#define DNI_FUER_RANG  -12
#define ERROR_MEMORIA  -13
#define FECHA_INVALIDA -14
#define EDAD_MENOR_10  -15
#define FECHA_AFIL_MAL -16
#define CUOTA_FUERA_RANG -17
#define ERROR_TUTOR    -18
#define ERROR_VALID    -19
#define ERROR_CATEGORIA -20
#define ERROR_STOCK    -21
#define ERROR_ARCHIVO  -22
#define ERROR_DNI_DUP  -23
#define ERROR_PLAN     -24
#define ERROR_GENERO   -25
#define ERROR_ID_DUP   -26
#define ERROR_DUPLICADO -27
#define ERROR_NOMBRE   -28

// TDA Indice
#define CANTIDAD_ELEMENTOS    100
#define INCREMENTO            1.3
#define OK                    1
#define ERROR                 0
#define NO_EXISTE            -1
#define NO_LLENO             -2

// Limite alquileres plan BASIC
#define MAX_ALQUILER_BASIC 2

///MACROS
#define miToLower(c) (((c)>='A'&&(c)<='Z')?((c)+('a'-'A')):(c))
#define miToUpper(c) (((c)>='a'&&(c)<='z')?((c)-('a'-'A')):(c))
#define esLetra(c)   ((((c)>='A'&&(c)<='Z')||((c)>='a'&&(c)<='z'))?1:0)
#define esAlpha(c)   (((((c)>='A'&&(c)<='Z'))||((c)>='a'&&(c)<='z'))||((c)>='0'&&(c)<='9')?1:0)

///PATH ARCHIVOS
#define PATH_MIEMPROS   "miembros.csv"
#define PATH_TITULOS    "titulos.csv"
#define PATH_ALQUILERES "alquiler.csv"

///STRUCTS
typedef struct
{
    int d, m, a;
} t_fecha;

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
} t_miembros;

typedef struct
{
    int  idPeli;
    char titulo[STRING];
    char genero[TAM_GENERO];
    int  stock;
} t_pelis;

typedef struct
{
    long dni;
    int  idPeli;
    int  cantAlquileres;
} t_alquiler;

typedef struct
{
    char tipoError[20];
    int  cantIncidencias;
    long clave;
} t_auditoria;

typedef struct
{
    void    *vindice;
    unsigned cantidad_elementos_actual;
    unsigned cantidad_elementos_maxima;
} t_indice;

/// ----------------------------- PROTOTIPOS ----------------------------------

/// TDA_FECHA
void    ingresarFecha(t_fecha *f);
bool    esFechaValida(const t_fecha *f);
int     cantDiaMes(int m, int a);
bool    esBisiesto(int a);
void    mostrarFecha(const t_fecha *f);
int     compararFecha(const t_fecha*, const t_fecha*);
t_fecha restarDiasAFecha(const t_fecha *f, int dias);
int     diferenciaEntreFechas(t_fecha*, t_fecha*);
int     diaDeLaSemana(t_fecha*);
t_fecha obtenerFechaProceso();
t_fecha parsear_fecha(const char *cad);

/// PROTOTIPOS INDICE TDA
int  indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo);
int  indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo);
int  indice_insertar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *));
int  indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *));
int  indice_buscar(const t_indice *indice, const void *registro, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *));
int  indice_vacio(const t_indice *indice);
int  indice_lleno(const t_indice *indice);
void indice_vaciar(t_indice *indice);
int  indice_cargar(const char *path, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *));

/// NORMALIZAR
char *crearCuil(long dni, char sexo);
int   calcularDigito(int *tipo, long dni);
char *normalizarNombre(char *nya);
char *normalizarNomPel(char *gen_o_tit);

/// VALIDACIONES
int  validarCorreo(char *correo);
void mostrarErrorCorreo(int codigo);
int  valEmailTut(char *emailT, t_fecha *fNac, t_fecha *fProc);
int  validarPlan(char *cat);
void validarCat(t_fecha *fProc, t_miembros *miembro);
int  validarDni(long dni);
int  validarSexo(char sexo);
int  validarUltimaCuota(t_fecha *fCuota, t_fecha *fAfil, t_fecha *fProc);
int  validarFechaAfil(t_fecha *fAfil, t_fecha *fNac, t_fecha *fProc);
int  validarFechaNac(t_fecha *fNac, t_fecha *fProc);
int  validarGenero(char *genero);
int  validarStock(int stock);

/// GESTION DE ARCHIVOS
void trozado(char *linea, t_miembros *m);
void trozado_peli(char *linea, t_pelis *p);
int  procesarMiembro(char *registro, t_miembros *miembro, t_fecha fechaProceso);
int  procesarPelicula(char *registro, t_pelis *peli);
void cargarDatos(t_indice *indMiembros, t_indice *indPelis, t_fecha fProc,
                 const char *pathMiembros, const char *pathPelis);
void MostrarArchivos(t_indice *indMiembros, t_indice *indPelis);

/// MENU
void MostrarMenu();
void EjecutarMenu(t_indice *indMiembros, t_indice *indPelis, t_indice *alquileres,
                  t_fecha fProc, const char *pathMiembros, const char *pathPelis,
                  const char *pathAlq);
t_fecha obtenerFechaProceso();

/// FUNCIONES DE COMPARACION
int comparar_dni(const void *dniA, const void *dniB);
int comparar_id_peli(const void *a, const void *b);
int comparar_nya(const void *a, const void *b);
int comparar_alquiler(const void *a, const void *b);
void limpiar_buffer();

/// OPERACIONES DEL MENU
void LeerTexto(char texto[], int largo);
int  AltaMiembros(t_indice *indice, t_fecha fProc);
int  AltaTitulo(t_indice *indice);
int  BajaMiembros(t_indice *indice, long dniBorrar);
int  BajaTitulo(t_indice *indice, int idBorrar);
int  ModificarMiembro(t_indice *indice, t_fecha fProc);
int  ModificarTitulo(t_indice *indice);
void MostrarInfoMiembro(t_indice *indice);
int  AlquilarTitulo(t_indice *indMiembros, t_indice *indPelis, t_indice *indAlq);
void ListadoPorDni(t_indice *indice);
void ListadoPorPlan(t_indice *indice);
void AlquilerPeli(t_indice *miembro, t_indice *peli, t_indice *alquileres, const char *NombreArchPelis, t_fecha fProc);
#endif // CINEFILIAHEADER_H_INCLUDED
