#include "titoliLIST.h"
#define S 20+1
typedef struct nodo *link;
struct nodo{
    TITOLO stock;
    link next;
};
struct titoliLIST{
    int n_titoli;
    link head;
};
static char buf[S];
static link new_node(TITOLO stock,link next){
    link node_x = malloc(sizeof (struct nodo));
    node_x->stock = stock;
    node_x->next = next;
    return node_x;
}
static void free_node(link node_x){
    TITOLO_free(node_x->stock);
    free(node_x);
}
static void liberaLista(link h){
    link t = h;
    if(h == NULL)
        return;
    while (h != NULL){
        t = h;
        free_node(h);
        h = t->next;
    }
}
static TITOLO searchList(link h,TITOLO_KEY key){
    link x;
    for(x = h; x!=NULL && strcmp(key,TITOLO_extractKey(x->stock)) >= 0; x = x->next){
        if(strcmp(key,TITOLO_extractKey(x->stock)) == 0)
            return x->stock;
    }
    return TITOLO_NULL();
}
static link SortListIns(link h,TITOLO stock){
    link x,p;
    if(h == NULL || strcmp(TITOLO_extractKey(stock),TITOLO_extractKey(h->stock)) < 0 ){
        h = new_node(stock,h);
        return h;
    }
    for(p=h,x=h->next; x !=NULL &&
        strcmp(TITOLO_extractKey(stock),TITOLO_extractKey(x->stock)) > 0;p=x,x=p->next);
    p->next  = new_node(stock,x);
    return h;
}
TITOLI TITOLI_init(){
    TITOLI stockList = malloc(sizeof(TITOLI));
    stockList->head = NULL;
    stockList->n_titoli = 0;
    return stockList;
}
void TITOLI_free(TITOLI stockList){
    liberaLista(stockList->head);
    stockList->n_titoli = 0;
    free(stockList);
}

void TITOLI_add(FILE *fin,TITOLI stockList){
    /*Inserimento in lista ordinato per chiave TITOLO_key*/
    TITOLO stock;
    fscanf(fin,"%s",buf);
    stock = searchList(stockList->head,buf);
    if(stock == TITOLO_NULL()){
        stock = TITOLO_init(buf);
        stockList->head = SortListIns(stockList->head,stock);
        stockList->n_titoli++;
    }
    assert(stock != NULL);
    TITOLO_addTransazioni(fin,stock);
}
void TITOLI_UI(TITOLI stockList,TITOLO_KEY key){
    TITOLO stock = searchList(stockList->head,key);
    if(stock == TITOLO_NULL()){
        printf("TITOLO NON TROVATO! ERRORE\n");
        return;
    }
    TITOLO_UI(stock);
}

void TITOLI_print(FILE *fout,TITOLI stockList){
    link h =stockList->head;
    while (h != NULL){
        TITOLO_storage(fout,h->stock,e_titolo);
        h = h->next;
    }
}
