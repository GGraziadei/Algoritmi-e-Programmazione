#ifndef PG_H_DEFINED
#define PG_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "equipArray.h"
#define SOGLIA 1
/*SOGLIA equip non ha effetto se posta a -infinito(INT_MIN)*/

/* quasi ADT personaggio; si noti che si tratta di un composto con riferimento 
a un equipArray di proprieta' */
typedef struct pg_s {
  char cod[LEN];
  char nome[LEN];
  char classe[LEN];
  stat_t b_stat, eq_stat;
  equipArray_t equip;
} pg_t;
typedef char* PG_KEY;
typedef enum {
    mode_pg,mode_pg_equip,mode_pg_stat,mode_pg_equip_stat
} mod_pg ;
/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pgp);
void pg_void (pg_t *pgp, int init_value);
/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(pg_t *pgp);

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray, mod_pg stampaMode);
/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray);

/* Si possono aggiungere altre funzioni se ritenute necessarie */
PG_KEY pg_getKey(pg_t *pgp);
int PG_KEY_eq (PG_KEY k1, PG_KEY k2);
#endif
