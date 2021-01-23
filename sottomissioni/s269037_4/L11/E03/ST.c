#include "ST.h"
struct st_tab{
    int n,nMax;
    node_key *node_info;
};
static int KEY_eq(ST_KEY k1, ST_KEY k2);
static node_key KEY_NULL(){
    node_key k;
    k.net = k.chiave = NULL;
    return k;
}
ST ST_init(int V){
    ST st = malloc(sizeof(struct st_tab));
    assert(st != NULL);
    st->n = 0;
    st->nMax = V;
    st->node_info = malloc(V*sizeof (node_key));
    return st;
}
int ST_count (ST st){
    return st->n;
}
void ST_insert(ST st, ST_KEY chiave, ST_KEY net){
    int i = st->n;
    if(st->n  >= st->nMax) {
        st->nMax  = 2*st->nMax;
        st->node_info = realloc(st->node_info, st->nMax * sizeof (node_key));
        assert(st->node_info != NULL);
    }
    while(i>=1 && strcmp(chiave,st->node_info[i - 1].chiave)<0) {
        st->node_info[i].chiave = st->node_info[i-1].chiave;
        st->node_info[i].net = st->node_info[i-1].net;
        i--;
    }
    st->node_info[i].chiave = strdup(chiave);
    st->node_info[i].net = strdup(net);
    assert(st->node_info[i].chiave != NULL && st->node_info[i].net !=NULL);
    st->n++;
}
int ST_search (ST st, ST_KEY chiave){
    int i;
    for(i=0; i<st->n && !KEY_eq(chiave,st->node_info[i].chiave); i++);
    return (i==st->n)?-1:i;
}
node_key ST_getKey(ST st, int index){
    if(index>=0 && index<st->n)
        return st->node_info[index];
    return KEY_NULL();
}

static int KEY_eq(ST_KEY k1, ST_KEY k2){
    int res = strcmp(k1,k2);
    return res==0?1:0;
}
void ST_free(ST st){
    int i;
    for(i=0; i<st->n; i++) {
        if (st->node_info[i].chiave != ST_initValue) {
            free(st->node_info[i].chiave);
            free(st->node_info[i].net);
        }
    }
    free(st->node_info);
    free(st);
}

