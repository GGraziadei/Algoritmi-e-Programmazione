#ifndef E3_INV_H
#define E3_INV_H
#define INVENTARIO "inventario.txt"
#define S 50+1
typedef enum {
    inv_file,inv_keyboard
}inv_add_from;
typedef struct {
    int hp,mp,atk,def,mag,spr;
}stat_t;
typedef struct t_int_t{
    char *name,*type;
    stat_t stat;
}inv_t;
typedef struct tabInv_t *link_tabInv;
struct tabInv_t{
    inv_t *vettInv;
    int nInv,maxInv;
};
link_tabInv init_tabInv();
link_tabInv libera_tabINV (link_tabInv t);
int linearSearchInv (link_tabInv tabInv, char *name);
stat_t init_stat(char *str);
stat_t statSetVOID ();
void w_add_INV (link_tabInv tabInv, inv_add_from source);
inv_t init_INV (char *str);
void free_INV(inv_t inv);
void w_info_obj(link_tabInv  tabInv);
void stampa_INV_i(inv_t *inv,FILE *out);
void stampa_INV(link_tabInv tabInv, FILE*out);
void  stampa_STAT (stat_t *stat, FILE *out);
#endif //E3_INV_H
