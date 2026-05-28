#include "Indice.h"
#include "cinefiliaHeader.h"

int indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
 

    /*
    FILE *maestro, *indi;
    int n=1;
    t_miembros miembro;
    t_reg_indice idx;
    
    if(maestro = fopen("miembros.dat","r")==NULL)
    {
        puts("Error al abrir archivo");
        getch();
        exit(1);
    }
    
    if(indi = fopen("indice.idx","w+b")==NULL)
    {
        puts("Error al abrir archivo");
        getch();
        exit(1);
    }
    
    rewind(maestro);
    
    while(fread(&miembro,sizeof(t_miembros),1,maestro))
    {
        idx.dni = miembro.dni;
        idx.nro_reg = n++;
        fwrite(&idx,sizeof(t_reg_indice),1,indi);
    }
    */
    

}


int indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    unsigned nuevaCap = (unsigned)(indice->cantidad_elementos_maxima * 1.3);

    void *aux = realloc(indice->vindice, nuevaCap * tamanyo);

    if(aux  == NULL)
        return ERROR_MEMORIA;
    
    indice->vindice = aux;

    indice->cantidad_elementos_maxima = nuevaCap;

    return EXITO;
}


int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    if(indice->cantidad_elementos_actual >= indice->cantidad_elementos_maxima)
    {

        indice_redimensionar(indice, indice->cantidad_elementos_maxima, tamanyo);

        if(indice->cantidad_elementos_actual>=indice->cantidad_elementos_maxima)
            return ERROR_MEMORIA; //significa que el realloc no funciono
    }

    char *base = (char *)indice->vindice; //lo paso a char para que la multiplicacion sea más fácil y se pueda saltar de uno en uno

    int i = indice->cantidad_elementos_actual - 1;

    //mientras mi elemento sea mayor a al nuevo voy para la derecha
    while(i >= 0 && cmp(base + (i*tamanyo), registro) > 0)
    {
        memcpy(base + ((i+1) * tamanyo), base + (i*tamanyo), tamanyo); //copio lo que esta en i a i+1
        i--; 
    }

    memcpy(base +((i+1) *tamanyo), registro, tamanyo);
    indice->cantidad_elementos_actual++;

    return EXITO;
}


int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int pos = indice_buscar(indice, registro, indice->cantidad_elementos_actual, tamanyo, cmp);

    if(pos == -1)
        return ERROR;
    
    char *base = (char *)indice->vindice;
    
    if((unsigned)pos < indice->cantidad_elementos_actual - 1)
    {
        void *destino = base + (pos*tamanyo);
        
        void *origen = base + ((pos +1) * tamanyo);
        
        size_t bytes_a_mover = (indice->cantidad_elementos_actual - pos - 1) * tamanyo;

        memmove(destino, origen, bytes_a_mover);
    }

    indice->cantidad_elementos_actual--;

    return EXITO;
}


int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    

}


int indice_vacio(const t_indice *indice)
{


}

int indice_lleno(const t_indice *indice)
{


}

void indice_vaciar(t_indice* indice)
{


}


int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
tamanyo, int (*cmp)(const void *, const void *))
{



}