#ifndef MAZO_H_INCLUDED
#define MAZO_H_INCLUDED

#include"carta.h"
#include<string.h>
#include<time.h>

#define OK 0//Me planteo crear un .h solo para los c�digos que est�n repetidos...
#define CANT_CARTAS_INVALIDA -1

typedef struct
{
    tCarta mazo[MAX_CARTAS];
    int tope;
}tMazo;

typedef void(fMost)(const void*);
void crearMazo(tMazo*p);
int inicializarMazo(tMazo*p);
void mezclarMazo(tMazo*p);
void recorrerMazo(tMazo*);//funci�n de prueba y bien fea.
void intercambiar(void*a,void*b,size_t tam);//esta funci�n me parece que tiene que ir a para en utilidades.h
#endif // MAZO_H_INCLUDED
