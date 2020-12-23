#include "ST.h"
struct st_tab{
    int n;
    ST_KEY *chiavi;
};
static int KEY_eq(ST_KEY k1, ST_KEY k2);
ST ST_init(int V, ST_KEY init_value){
    int i;
    ST st = malloc(sizeof(struct st_tab));
    assert(st != NULL);
    st->n = V;
    st->chiavi = malloc(V*sizeof (ST_KEY));
    for(i=0; i<V; i++)
        st->chiavi[i] = init_value;
    return st;
}
int ST_count (ST st){
    int i;
    for(i=0; i<st->n && st->chiavi[i]!=ST_initValue; i++);
    return i;
}
void ST_insert(ST st, int index, ST_KEY chiave){
    st->chiavi[index] = strdup(chiave);
    assert(st->chiavi[index] != NULL);
}
int ST_search (ST st, ST_KEY chiave){
    int i;
    for(i=0; i<st->n && !KEY_eq(chiave,st->chiavi[i]); i++);
    return (i==st->n)?-1:i;
}
ST_KEY ST_getKey(ST st, int index){
    if(index>=0 && index<st->n)
        return st->chiavi[index];
}
static int KEY_eq(ST_KEY k1, ST_KEY k2){
    int res = strcmp(k1,k2);
    return res==0?1:0;
}
void ST_free(ST st){
    int i;
    for(i=0; i<st->n; i++)
        if(st->chiavi[i]!=ST_initValue)
            free(st->chiavi[i]);
    free(st);
}