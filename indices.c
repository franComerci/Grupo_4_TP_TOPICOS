#include "cinefiliaHeader.h"

int indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->vindice = malloc(nmemb * tamanyo);
    if(indice->vindice == NULL)
        return ERROR_MEMORIA;

    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = nmemb;

    return OK;
}

int indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    unsigned nuevaCap = (unsigned)(indice->cantidad_elementos_maxima * INCREMENTO);

    void *aux = realloc(indice->vindice, nuevaCap * tamanyo);

    if(aux  == NULL)
        return ERROR_MEMORIA;

    indice->vindice = aux;

    indice->cantidad_elementos_maxima = nuevaCap;

    return OK;
}

//uso memcpy para que quede generico, para poder usar esta funcion con miembros y peliculas
int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    if(indice->cantidad_elementos_actual >= indice->cantidad_elementos_maxima)
    {

        indice_redimensionar(indice, indice->cantidad_elementos_maxima, tamanyo);

        if(indice->cantidad_elementos_actual>=indice->cantidad_elementos_maxima)
            return ERROR_MEMORIA; //significa que el realloc no funciono
    }

    char *base = (char *)indice->vindice; //lo paso a char para que la multiplicacion sea m�s f�cil y se pueda saltar de uno en uno

    int i = indice->cantidad_elementos_actual - 1;

    //arranco desde el final al principio
    //mientras mi elemento sea mayor a al nuevo voy para la derecha
    while(i >= 0 && cmp(base + (i*tamanyo), registro) > 0)
    {
        memcpy(base + ((i+1) * tamanyo), base + (i*tamanyo), tamanyo); //copio lo que esta en i a i+1
        i--;
    }


    memcpy(base +((i+1) *tamanyo), registro, tamanyo); // aca copio el nuevo registro a lo ultimo
    indice->cantidad_elementos_actual++;

    return OK;
}

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int pos = indice_buscar(indice, registro, indice->cantidad_elementos_actual, tamanyo, cmp);

    if(pos == NO_EXISTE)
        return ERROR;

    char *base = (char *)indice->vindice;

    if((unsigned)pos < indice->cantidad_elementos_actual - 1)
    {
        //muevo lo que esta a la derecha a la izquierda (lo piso)
        void *destino = base + (pos*tamanyo);

        void *origen = base + ((pos +1) * tamanyo);

        size_t bytes_a_mover = (indice->cantidad_elementos_actual - pos - 1) * tamanyo;

        memmove(destino, origen, bytes_a_mover); // sobreescribo el destino con origen
    }

    indice->cantidad_elementos_actual--;

    return OK;
}

//uso busqueda binaria
int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int inicio = 0;
    //int fin = indice->cantidad_elementos_actual - 1; no lo necesito porque tengo nmemb como parametro
    char *base = (char*)indice->vindice; //tengo que pasarlo a char pq es void
    while(inicio <= nmemb)
    {
        int medio = inicio + (nmemb - inicio)/2; //posicion central

        void *elementoMedio = base + (medio *tamanyo);
        int comp = cmp(registro, elementoMedio);
        if(comp == 0) // si la comparacion es 0 significa que cuando se restaron eran iguales
        {
            return medio; // devuelvo la pos
        }
        else if(comp > 0)
        {
            inicio = medio + 1; //el buscado es menor, entonces me muevo para la derecha
        }
        else
            nmemb = medio - 1; // el buscado es mayor, entonces me muevo para la izq
    }

    return NO_EXISTE;
}

int indice_vacio(const t_indice *indice)
{
    return indice->cantidad_elementos_actual == 0? OK : ERROR;
}

int indice_lleno(const t_indice *indice)
{
    return (indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima? OK :  NO_LLENO);

}

void indice_vaciar(t_indice* indice)
{
    indice->cantidad_elementos_actual = 0; //???????????? no s� si hay que hacer un free ac�
}

int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    //abro el archivo en modolectura
    FILE *arch = fopen(path, "r");
    if(arch == NULL)
        return ERROR;
    fread(vreg_ind, tamanyo, 1, arch);
    while(!feof(arch))
    {
        int regProcesado = 0;
        char *base = (char *)indice->vindice;

        if(indice->cantidad_elementos_actual > 0)
        {
            void *anterior = base + ((indice->cantidad_elementos_actual - 1) * tamanyo); //TENGO QUE COMPARAR CON EL ULTIMO INSERTADO PORQUE ESTA ORDENADO, no vale la pena usar la funcion de busqueda

            int resul = cmp(vreg_ind, anterior);

            if(resul < 0)
            {
                //significa que si lo meto al final est� desordenado entonces tengo que llamar a insertar para que quede ordenado
                indice_insertar(indice, vreg_ind, tamanyo, cmp);
                regProcesado = 1;

            }
            else if(resul == 0)//significa que es repetido
                regProcesado = 1;
        }

        //aca me baso en que viene ordenado o que es el primer elemento
        if(regProcesado == 0)
        {
            if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
            {
                indice_redimensionar(indice, indice->cantidad_elementos_maxima, tamanyo);

                if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
               {
                   fclose(arch);
                   return ERROR;
               }
               base = (char *)indice->vindice; ///agregado por gemini
            }
        }
        void *destino = base + (indice->cantidad_elementos_actual * tamanyo);

        memcpy(destino, vreg_ind, tamanyo);
        indice->cantidad_elementos_actual++;

        fread(vreg_ind, tamanyo, 1, arch);
    }

    fclose(arch);

    return OK;
}

