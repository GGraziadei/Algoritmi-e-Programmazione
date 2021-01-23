#ifndef E1_ST_H
#define E1_ST_H
#include <stdlib.h>
#include <assert.h>
#include <string.h>
typedef struct st_s *ST;
typedef char * ST_key;
int ST_GETindex(ST st_t, ST_key chiave);
ST_key ST_GETkey (ST st_t, int index);
int ST_count(ST st_t);
void ST_insert(ST st_t, ST_key chiave);
void ST_free(ST st_t);
ST ST_init(int V);
#endif //E1_ST_H
