#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define EQUIP_SLOT 8
#define VUOTO -100
/*Definisce se l'aggiornamento di un equipaggiamento permette ADD/REMOVE multipli o meno*/
#define MULTIPLE_UPDATES 0
#include "invArray.h"

/* ADT di prima classe collezione di oggetti di equipaggiamento */
typedef struct equipArray_s *equipArray_t;

/* creatore e disruttore */
equipArray_t equipArray_init();
void vettEq_void(int *vettEq);
void equipArray_free(equipArray_t equipArray);

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray);

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray);
/* modifica equipaggiamento scegliendo un oggetto da inventario, ritorna i DELTA statistiche*/
stat_t equipArray_update(equipArray_t equipArray, invArray_t invArray);
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index);
/*contrario della precedente funzione*/
int equipArray_getIndexByEquip(equipArray_t equipArray, int index);
/* Si possono aggiungere altre funzioni se ritenute necessarie */
void equipArray_cpy (equipArray_t dest,equipArray_t src);
#endif
