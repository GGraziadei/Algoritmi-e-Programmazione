#include "pgList.h"

struct nodePg_t{
    link next;
    pg_t pg;
};
struct pgList_s{
    link head,tail;
    int nPg;
};
static link new_node(pg_t pg,link next);
static link free_node(link node_x);

pgList_t pgList_init(){
    pgList_t pgList = malloc(sizeof (struct pgList_s));
    pgList->head = pgList->tail =  NULL;
    pgList->nPg = 0;
    return pgList;
}
void pgList_free(pgList_t pgList){
    libera_lista(pgList);
    if(pgList->nPg == 0 && pgList->head == NULL)
        free(pgList);
}
void libera_lista (pgList_t pgList){
    if(pgList->head == NULL)return;
    link t = NULL,x = pgList->head;
    while (x != NULL && t == NULL){
        t = x;
        x = x->next;
        t = free_node(t);
        pgList->nPg--;
    }
    pgList->head = t;
}
static link new_node(pg_t pg,link next){
    link node_x = malloc(sizeof (struct nodePg_t));
    node_x->pg = pg;
    node_x->next = next;
    return node_x;
}

static link free_node(link node_x){
    pg_clean(&node_x->pg);
    free(node_x);
    return  NULL;
}

void pgList_read(FILE *fp, pgList_t pgList){
/*Inserzione in coda*/
    pg_t pg;
    assert(fp != NULL);
    while (!feof(fp) && pg_read(fp,&pg))
        pgList_insert(pgList,pg);
}
void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    pgList_printR(fp, pgList->head,invArray);
}
void pgList_printR(FILE *fp, link node_x, invArray_t invArray){
    if(node_x == NULL) return;
    pg_print(fp,&node_x->pg,invArray,mode_pg);
    pgList_printR(fp,node_x->next,invArray);
}
void pgList_insert(pgList_t pgList, pg_t pg){
    if(pgList_searchByCode(pgList,pg_getKey(&pg)) != NULL)
        pgList_remove(pgList,pg_getKey(&pg)); /*Evito duplicati, in caso di immissione
 * codice già presente in struttura dati aggiorno il PG*/
    if(pgList->head == NULL)
        pgList->head = pgList->tail = new_node(pg,pgList->tail);
    else {
        pgList->tail->next = new_node(pg, NULL);
        pgList->tail = pgList->tail->next;
    }
    pgList->nPg ++;
    //fprintf(stdout,"ADD!\n");
}
void pgList_remove(pgList_t pgList,PG_KEY cod){
    link prev,node_x = pgList->head;
    if ( node_x == NULL) return;
    prev = NULL;
    while (node_x!=NULL && PG_KEY_eq(pg_getKey(&node_x->pg),cod) != 1){
        prev = node_x;
        node_x = node_x->next;
    }
    if (node_x == NULL)fprintf(stdout,"CODICE NON TROVATO!\n");
    else{
        if (node_x == pgList->head ) pgList->head = node_x ->next;
        else prev->next = node_x->next;
        if (node_x == pgList->tail ) pgList->tail = prev;
        free_node(node_x);
        pgList->nPg--;
        //fprintf(stdout,"DELETE!\n");
    }
}
pg_t *pgList_searchByCode(pgList_t pgList, PG_KEY cod){
    link node_x = pgList->head;
    while (node_x!=NULL && PG_KEY_eq(pg_getKey(&node_x->pg),cod) != 1)
        node_x = node_x->next;
    if(node_x == NULL )return NULL;
    else return &node_x->pg;
}