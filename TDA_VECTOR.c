#include "CinefiliaFunctions.h"

//TDA vector
void vector_crear(t_vector *v)//inicializo el vec y las variables
{
    v->vec = NULL;
    v->cantidad = 0;
    v->capacidad = 0;
}

int vector_insertar(t_vector *v, t_miembros nuevo) //manejo como entran los nuevos miembros
{
    if(v->cantidad >= v->capacidad)
    {
        int nuevaCap =(v->capacidad == 0)? 10: v->capacidad * 2; // primero le asigno 10 si es 0, si no lo duplico
        t_miembros *aux= (t_miembros *)realloc(v->vec, nuevaCap * sizeof(t_miembros)); // calculo la nueva capacidad si es que se lleno el vec

        if(aux == NULL)
            return ERROR_MEMORIA;

        v->vec = aux;
        v->capacidad = nuevaCap;
    }

    v->vec[v->cantidad] = nuevo; // lo pongo en cantidad suponiendo que siempre se inserta al final
    v->cantidad++;

    return EXITO;
}

void vector_destruir(t_vector *v)
{
    if(v->vec != NULL)
    {
        free(v->vec);
        v->vec = NULL;
    }

    v->cantidad = 0;
    v->capacidad = 0;
}


