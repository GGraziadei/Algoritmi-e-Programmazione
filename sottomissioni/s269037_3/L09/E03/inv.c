#include <stdio.h>
#include "string_custom.h"
#include <stdlib.h>
#include <assert.h>
#include "inv.h"
#define DBG 0
link_tabInv init_tabInv(){
    link_tabInv tabInv = (link_tabInv) malloc(sizeof(struct tabInv_t));
    assert (tabInv != NULL);
    tabInv->vettInv = NULL;
    tabInv->maxInv = tabInv->nInv = 0;
    return tabInv;
}
link_tabInv libera_tabINV (link_tabInv t){
    int i;
    for(i=0; i<t->maxInv; i++)
        free_INV(t->vettInv[i]);
    free(t->vettInv);
    return NULL;
}


void w_add_INV (link_tabInv tabInv, inv_add_from source){
    FILE *in;
    char tmp_str[3 * S];
    if(source == inv_file) {
        in = fopen(INVENTARIO, "r");
        assert(in != NULL);
        fgets(tmp_str,3*S,in);
        sscanf(tmp_str,"%d",&tabInv->nInv);
        tabInv->maxInv = tabInv->nInv;
        tabInv->vettInv = malloc(tabInv->nInv*sizeof(inv_t));
        tabInv->nInv = 0;
        while (fgets(tmp_str,3*S,in) != NULL)
            tabInv->vettInv[tabInv->nInv ++] = init_INV(tmp_str);
        fclose(in);
    }
#if DBG
    printf("\nINVENTARIO: \n");
    stampa_INV(tabInv,stdout);
#endif
}

inv_t init_INV (char *str){
    int i;
    char nome[S],type[S];
    inv_t inv;
    sscanf(str,"%s %s%n",nome,type,&i);
    inv.name = strdup(nome);
    inv.type = strdup(type);
    assert(inv.name != NULL && inv.type != NULL);
    inv.stat = init_stat(&(str[i]));
    return inv;
}
void free_INV(inv_t inv){
    free(inv.name);
    free(inv.type);
}

void w_info_obj(link_tabInv  tabInv){
    char nome[S];
    int id;
    scanf("%s",nome);
    toMinuscolo(nome);
    id = linearSearchInv(tabInv,nome);
    if(id != -1){
        stampa_INV_i(&tabInv->vettInv[id],stdout);
        stampa_STAT(&tabInv->vettInv[id].stat,stdout);
    }
    else
        printf("OGGETTO NON TROVATO\n");
}

int linearSearchInv (link_tabInv tabInv, char *name){
    char *test;
    int i,id =-1;
    for (int i = 0; i < tabInv->nInv && id==-1; i++){
        test = strdup(tabInv->vettInv[i].name);
        toMinuscolo(test);
        if(!strcmp(test,name))
            id = i;
        free(test);
    }
    return id;
}

stat_t init_stat(char *str){
    stat_t stat_x = statSetVOID();
    if(str == NULL){
        printf("Inserire STAT (hp mp atk def mag spr, separati da spazio): ");
        assert(fscanf(stdin,"%d %d %d %d %d %d",&stat_x.hp,&stat_x.mp,
                      &stat_x.atk,&stat_x.def,&stat_x.mag,&stat_x.spr)==6);
        return stat_x;
    }else {
        assert(sscanf(str, "%d %d %d %d %d %d", &stat_x.hp, &stat_x.mp,
                      &stat_x.atk, &stat_x.def, &stat_x.mag, &stat_x.spr) == 6);
        return stat_x;
    }
}
stat_t statSetVOID (){
    stat_t stat;
    stat.hp = stat.mp = stat.atk = stat.def = stat.mag = stat.spr = 0;
    return stat;
}

/*Stampa*/
void stampa_INV(link_tabInv tabInv, FILE*out){
    int i;
    for (int i = 0; i < tabInv->maxInv; ++i){
        fprintf(out,"%d ->",i+1);
        stampa_INV_i(&tabInv->vettInv[i],stdout);
    }
}
void stampa_INV_i(inv_t *inv,FILE *out){
    fprintf(out,"%s : %s\n",inv->name,inv->type);
}
void  stampa_STAT (stat_t *stat, FILE *out){
    fprintf(out,"hp: %d\nmp: %d\natk: %d\ndef: %d\nmag: %d\nspr: %d\n",stat->hp,stat->mp,stat->atk,stat->def,stat->mag,stat->spr);
}
