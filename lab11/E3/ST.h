#ifndef E3_ST_H
#define E3_ST_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define ST_initValue NULL
typedef struct st_tab *ST;
typedef char* ST_KEY;
typedef struct {
    ST_KEY chiave;
    ST_KEY net;
}node_key;
/*Tabella dei simboli dinamica la cui chiave è di tipo node_key */
ST ST_init(int V);
int ST_search (ST st, ST_KEY chiave);
node_key ST_getKey(ST st, int index);
void ST_free(ST st);
int ST_count (ST st);
void ST_insert(ST st, int index, ST_KEY chiave, ST_KEY net);

#endif //E3_ST_H
