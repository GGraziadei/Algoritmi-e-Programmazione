#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#define S 50+1
#define PERSONAGGI "pg.txt"
#define OGGETTI "inventario.txt"

typedef struct _stat_t{
    int hp,mp,atk,def,mag,spr;
}stat_t;
typedef struct t_int_t{
    char *name,*type;
    stat_t stat;
}inv_t;
typedef struct _tabEquip_t{
    short int status;
    inv_t **vettEq;
}tabEquip_t;
typedef struct _pg_t{
 int id;
 char *name,*class;
 stat_t stat;
}pg_t;
typedef struct nodoPg_t *link_nodoPg;
struct nodoPg_t{
    pg_t pg;
    link_nodoPg next;
};
typedef struct tabInv_t *link_tabInv;
struct tabInv_t{
    inv_t *vettInv;
    int nInv,maxInv;
};
typedef struct tabPg_t *link_tabPg;
struct tabPg_t{
    link_nodoPg headPg,tailPg;
    int nPg;
};

link_tabPg init_tabPg();
link_tabInv init_tabInv();
stat_t statSetVOID ();
pg_t pgSetVOID();

int main() {
    link_tabPg tabPg = init_tabPg();
    link_tabInv tabInv = init_tabInv();



    return 0;
}

link_tabPg init_tabPg(){
    link_tabPg tabPg = (link_tabPg) malloc(sizeof(struct tabPg_t));
    assert (tabPg != NULL);
    tabPg->headPg = tabPg->tailPg = NULL;
    tabPg->nPg = 0;
    return tabPg;
}

link_tabInv init_tabInv(){
    link_tabInv tabInv = (link_tabInv) malloc(sizeof(struct tabInv_t));
    assert (tabInv != NULL);
    tabInv->vettInv = NULL;
    tabInv->maxInv = tabInv->nInv = 0;
    return tabInv;
}

pg_t pgSetVOID(){
    pg_t pg;
    pg.stat = statSetVOID();
    pg.name = pg.class = NULL;
    pg.id = -1;
}

stat_t statSetVOID (){
    stat_t stat;
    stat.hp = stat.mp = stat.atk = stat.def = stat.mag = stat.spr = 0;
    return stat;
}