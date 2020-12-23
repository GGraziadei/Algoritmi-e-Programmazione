#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define LEN 100
#define MIN_STAT 1

/* quasi ADT statistiche */
typedef struct stat_s {
  int hp, mp, atk, def, mag, spr;
} stat_t;

/* quasi ADT oggetto di inventario */
typedef struct inv_s {
  char nome[LEN];
  char tipo[LEN];
  stat_t stat;
} inv_t;
typedef char* INV_KEY;
typedef enum {
    mode_inv,mode_inv_stat
} mod_inv ;
/* funzioni di input/output delle statistiche */
void stat_read(FILE *fp, stat_t *statp);
void stat_void (stat_t *statp,int init_value);
void stat_print(FILE *fp, stat_t *statp, int soglia);

/* funzioni di input/output di un oggetto dell'inventario */
void inv_void(inv_t *invp, int init_value);
int inv_read(FILE *fp, inv_t *invp);
void inv_print(FILE *fp, inv_t *invp,mod_inv stampaMode);
INV_KEY inv_getKey(inv_t *invp);
int INV_KEY_eq (INV_KEY k1, INV_KEY k2);
/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp);

/* Si possono aggiungere altre funzioni se ritenute necessarie*/
int check_stat (stat_t *s,int soglia);
stat_t stat_sum (stat_t *s1, stat_t *s2);
stat_t stat_min (stat_t *s1, stat_t *s2);
#endif
