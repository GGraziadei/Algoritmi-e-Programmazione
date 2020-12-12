//
// Created by Gianluca Graziadei on 30/11/2020.
//
#ifndef E3_PG_H
#define E3_PG_H
#include "inv.h"
#define PERSONAGGI "pg.txt"
#define S 50+1
#define MAX_EQUIP 8
typedef enum {
    pg_file,pg_keyboard
}pg_add_from;

typedef struct tabEquip_t *link_tabEquip;
struct tabEquip_t{
    short int status,inUso;
    inv_t *vettEq[MAX_EQUIP];
};
typedef struct {
    int id;
    char *name,*class;
    stat_t stat;
    link_tabEquip equip;
}pg_t;
typedef struct nodoPg_t *link_nodoPg;
struct nodoPg_t{
    pg_t pg;
    link_nodoPg next;
};
typedef struct tabPg_t *link_tabPg;
struct tabPg_t{
    link_nodoPg headPg,tailPg;
    int nPg;
};
typedef int PG_KEY;

link_tabPg init_tabPg();
link_tabPg  libera_tabPG (link_tabPg t);
link_nodoPg liberaLista_PG (link_nodoPg h);

link_nodoPg new_node_PG (link_nodoPg p_next, pg_t x_pg);
void free_node_PG (link_nodoPg p);
link_nodoPg linearSearchList (link_tabPg tabPg,PG_KEY id);
void listInsTailFast(link_tabPg tabPg,pg_t pg);
int listDelID (link_tabPg  tabPg,PG_KEY id);

void pgSetVOID(pg_t *pg);
pg_t init_PG (char *tmp_str);
link_tabEquip init_tabEquip();
link_tabEquip  free_tabEquip (link_tabEquip t);
void free_PG (pg_t *p_pg);

int PG_KEYcompare (PG_KEY k1, PG_KEY k2);
PG_KEY PG_KEYget(FILE *in, int init);

void w_add_PG (link_tabPg,pg_add_from);
void add_PG (pg_t pg, link_tabPg tabPg);
void w_info_PG (link_tabPg tabPg);
void w_delete_PG(link_tabPg tabPg);

void w_stat (link_tabPg tabPg);
stat_t sum_stat_now(stat_t stat_now,link_nodoPg x);

void w_update_Equip (link_tabInv tabInv, link_tabPg tabPg);
void w_remove_Equip (link_tabInv tabInv, link_tabPg tabPg);
void add_equip (link_nodoPg x, int indice, link_tabInv tabInv);
int remove_equip(link_nodoPg x);

void stampa_PG(pg_t *pg,FILE *out);
void stampa_PG_LIST(link_nodoPg l,FILE *out);
void stampaEquip (link_nodoPg x, FILE *out);
#endif //E3_PG_H
