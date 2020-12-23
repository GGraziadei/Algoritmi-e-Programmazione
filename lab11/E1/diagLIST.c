#include "diagLIST.h"
typedef struct node *link;
struct node{
    DIAG d;
    link next;
};
struct diagList{
    link head;
    int nDiag;
};
static link liberaLista(link h);
static void freeNode(link x);
static link new_node(DIAG d,link next);
DIAGLIST DIAGLIST_init(){
    DIAGLIST dl = malloc(sizeof (struct diagList));
    dl->head = NULL;
    dl->nDiag = 0;
    return dl;
}
static link liberaLista(link h){
    link tmp = h;
    if (h == NULL )return NULL;
    while(h != NULL){
        tmp = h->next;
        freeNode(h);
        h = tmp;
    }
    assert(h == NULL);
    return h;
}
void DIAGLIST_free(DIAGLIST dl){
    dl->head = liberaLista(dl->head);
    free(dl);
}

static link new_node(DIAG d,link next){
    link node_x = malloc(sizeof (struct node));
    assert(node_x != NULL);
    node_x->next = next;
    node_x->d = d;
    return node_x;
}
static void freeNode(link x){
    free(x->d.elments_index);
    free(x);
}
DIAG DIAG_init(int N, int *index_vector,float value,int livello){
    DIAG d;
    int i;
    d.elments_index = malloc(N*sizeof (int));
    d.value = value;
    d.num = N;
    d.livello = livello
    for(i=0; i<N; i++) d.elments_index[i] = index_vector[i];
    return d;
}
void DIAGLIST_add(int *index_vector,int N,float value,int livello, DIAGLIST dl,ELMENTS eA){
    /*Inserzione in testa*/
    DIAG d = DIAG_init( N,  index_vector, value,livello);
    link prev,x=dl->head;
    dl->nDiag++;
    if(x == NULL || x->d.value < d.value){
        dl->head = new_node(d,dl->head);
        return;
    }
    for(prev=x,x=x->next;x!=NULL && d.value<x->d.value;prev=x,x=prev->next);
    prev->next = new_node(d,x);
}

