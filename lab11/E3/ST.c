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
    st->node_info = malloc(V*sizeof (ST_KEY));
    return st;
}
int ST_count (ST st){
    return st->n;
}
void ST_insert(ST st, int index, ST_KEY chiave, ST_KEY net){
    int i;
    st->node_info[st->n++] = KEY_NULL();
    if(st->n >= st->nMax) {
        st->nMax  = 2*st->nMax;
        st->node_info = realloc(st->node_info, st->nMax * sizeof (node_key));
    }
    if (index > 0){
        /*Cerco l'indice a cui assegnare il puntatore*/
        for (i=0; i<index && strcmp(chiave,st->node_info[i].chiave) >= 0; i++);
        index = i;
        i = st->n - 1;
        while (i > index){
            /*Scambio di puntatori a memoria allocata al fine di garantire ordine alfabetico*/
            st->node_info[i].chiave = st->node_info[i-1].chiave;
            st->node_info[i].net = st->node_info[i-1].net;
            i--;
        }
    }
    st->node_info[index].chiave = strdup(chiave);
    st->node_info[index].net = strdup(net);
    assert(st->node_info[index].chiave != NULL && st->node_info[index].net !=NULL);

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

