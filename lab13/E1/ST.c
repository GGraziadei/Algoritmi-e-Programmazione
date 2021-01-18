#include "ST.h"
struct st_s {
    int N,maxN;
    ST_key *array;
};
static int ST_linearSearch(ST st_t, ST_key chiave){
    int i;
    for(i=0; i<st_t->N && strcmp(chiave,st_t->array[i]) != 0; i++);
    if( i < st_t->N)return i;
    return -1;
}
ST ST_init(int V){
    ST st_t = malloc(sizeof (struct st_s));
    assert(st_t != NULL);
    st_t->maxN = V;
    st_t->array = malloc(st_t->maxN * sizeof (ST_key));
    st_t->N = 0;
    assert(st_t->array != NULL);
    return st_t;
}
void ST_free(ST st_t){
    int i;
    for(i=0; i<st_t->N; i++)
        free(st_t->array[i]);
    free(st_t->array);
    free(st_t);
}
void ST_insert(ST st_t, ST_key chiave){
    if(st_t->N + 1 <= st_t->maxN)
        st_t->array[st_t->N++] = strdup(chiave);
    assert(st_t->array[st_t->N - 1 ]!= NULL);
}
int ST_count(ST st_t){return st_t->N;}
ST_key ST_GETkey (ST st_t, int index){return st_t->array[index];}
int ST_GETindex(ST st_t, ST_key chiave){return ST_linearSearch(st_t,chiave);}





