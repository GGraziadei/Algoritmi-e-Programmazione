#ifndef E3_ST_H
#define E3_ST_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define ST_initValue NULL
typedef struct st_tab *ST;
typedef char* ST_KEY;
ST ST_init(int V, ST_KEY init_value);
int ST_search (ST st, ST_KEY chiave);
ST_KEY ST_getKey(ST st, int index);
void ST_free(ST st);
int ST_count (ST st);
void ST_insert(ST st, int index, ST_KEY chiave);

#endif //E3_ST_H
