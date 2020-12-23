#include <stdio.h>
#include "string_custom.h"
#include <stdlib.h>
#include <assert.h>
#include "pg.h"
char buf[3*S];
#define DBG 0
link_tabPg init_tabPg(){
    link_tabPg tabPg = (link_tabPg) malloc(sizeof(struct tabPg_t));
    assert (tabPg != NULL);
    tabPg->headPg = tabPg->tailPg = NULL; /*LISTA VUOTA*/
    tabPg->nPg = 0;
    return tabPg;
}

link_tabPg  libera_tabPG (link_tabPg t){
    t->tailPg = t->headPg = liberaLista_PG(t->headPg);
    t->tailPg = 0;
    return NULL;
}

link_nodoPg liberaLista_PG (link_nodoPg h){
    if(h->next == NULL)
        return NULL;
    h->next = liberaLista_PG(h->next);
    free_PG(&(h->pg));
    free_node_PG(h);
    return NULL;
}

link_nodoPg new_node_PG (link_nodoPg p_next, pg_t x_pg){
    link_nodoPg x  = (link_nodoPg) malloc(sizeof(struct nodoPg_t));
    assert(x != NULL);
    x->pg = x_pg;
    x->next = p_next;
    return x;
}
void free_node_PG (link_nodoPg p){
    free(p);
}

void listInsTailFast(link_tabPg tabPg,pg_t pg){
    if(tabPg->headPg == NULL)
        tabPg->headPg = tabPg->tailPg = new_node_PG(tabPg->tailPg,pg);
    else{
        tabPg->tailPg->next = new_node_PG(NULL,pg);
        tabPg->tailPg = tabPg->tailPg->next;
    }
}
int listDelID (link_tabPg tabPg,PG_KEY id){
    link_nodoPg x,p;
    if(tabPg->headPg == NULL)
        return 0;
    for (x = tabPg->headPg, p=NULL; x!=NULL; p=x, x = x->next)
        if(PG_KEYcompare(x->pg.id,id)==1){
            printf("DELETE: ");
            stampa_PG(&x->pg,stdout);
            if(x==tabPg->headPg)
                tabPg->headPg = x->next;
            else
                p->next = x->next;
            if(tabPg->tailPg == x)
                tabPg->tailPg = p;
            free_PG(&(x->pg));
            free_node_PG(x);
            return 1;
        }
    return 0;
}
link_nodoPg linearSearchList (link_tabPg tabPg,PG_KEY id){
    link_nodoPg x=tabPg->headPg;
    while (x != NULL && PG_KEYcompare(x->pg.id,id)!=1 )
        x = x->next;
    return x;
}

/* NODO*/
void w_add_PG (link_tabPg tabPg, pg_add_from source){
    FILE *in;
    pg_t x;
    if(source == pg_file) {
        in = fopen(PERSONAGGI, "r");
        assert(in != NULL);
        while (fgets(buf, 3 * S, in) != NULL) {
            x = init_PG(buf);
            add_PG(x, tabPg);
        }
        fclose(in);
    }
    else if(source == pg_keyboard){
        x = init_PG(NULL);
        add_PG(x,tabPg);
    }
#if DBG
    printf("LISTA PERSONAGGI\n");
    stampa_PG_LIST(tabPg->headPg,stdout);
#endif
}
void add_PG (pg_t pg, link_tabPg tabPg){
    listInsTailFast(tabPg,pg);
    tabPg->nPg++;
}
void w_delete_PG(link_tabPg tabPg){
    PG_KEY id = PG_KEYget(stdin,0);
    if(listDelID(tabPg,id) == 0)
        printf("NESSUN PG eliminato.\nINSERIRE CODICE VALIDO!\n");
    printf("LISTA AGGIORNATA\n");
    stampa_PG_LIST(tabPg->headPg,stdout);
}

void w_info_PG(link_tabPg tabPg){
    PG_KEY id = PG_KEYget(stdin,0);
    link_nodoPg x;
    x = linearSearchList(tabPg,id);
    if( x == NULL){
        printf("PG non presente in lista.\nINSERIRE CODICE VALIDO!\n");
        return;
    }
    stampa_PG(&x->pg,stdout);
    stampa_STAT(&x->pg.stat,stdout);
    stampaEquip(x,stdout);
}
void pgSetVOID(pg_t *pg){
    pg->stat = statSetVOID();
    pg->name = pg->class = NULL;
    pg->equip = NULL;
    pg->id = NULL;
}

pg_t init_PG (char *tmp_str){
    char id[S],nome[S],class[S];
    int i;
    pg_t pg_x;
    pgSetVOID(&pg_x);
    if(tmp_str == NULL){
        pg_x.id = PG_KEYget(stdin,1);
        printf("Inserire NOME: ");
        scanf("%s",nome);
        printf("Inserire CLASSE: ");
        scanf("%s",class);
    }
    else {
        sscanf(tmp_str, "%s %s %s%n", id, nome, class, &i);
        pg_x.id = strdup(id);
    }
    pg_x.name = strdup(nome);
    pg_x.class = strdup(class);
    assert(pg_x.name != NULL && pg_x.class != NULL);
    if(tmp_str == NULL)
        pg_x.stat = init_stat(NULL);
    else
        pg_x.stat = init_stat(&(tmp_str[i]));
    pg_x.equip = NULL;
    return pg_x;
}

link_tabEquip init_tabEquip(){
    int i;
    link_tabEquip tab = malloc(sizeof(struct tabEquip_t));
    tab->status = tab->inUso = 0;
    for (int i = 0; i < MAX_EQUIP; ++i)
        tab->vettEq[i] = NULL;
    return tab;
}
link_tabEquip  free_tabEquip (link_tabEquip t){
    if(t != NULL)
        free(t);
    return NULL;
}
void free_PG (pg_t *p_pg){
    free(p_pg->id);
    free(p_pg->class);
    free(p_pg->name);
    p_pg->equip = free_tabEquip(p_pg->equip);
}
PG_KEY PG_KEYget(FILE *in,int init){
    PG_KEY k;
    do {
        if(init)printf("Inserire CODICE (PGXXXX): ");
        fscanf(in,"%s",buf);
        assert(buf != NULL);
        toMaiuscolo(buf);
    } while (strlen(buf)!=PG_KEY_L || strncmp(buf,"PG",2)!=0);
    k = strdup(buf);
    return k;
}
int PG_KEYcompare (PG_KEY k1, PG_KEY k2){
    return 1-(strcmp(k1,k2));
}

/*STAT_PG*/
void w_stat (link_tabPg tabPg){
    link_nodoPg  x;
    PG_KEY id = PG_KEYget(stdin,0);
    x = linearSearchList (tabPg,id);
    if(x == NULL){
        printf("Codice personaggio non presente in lista\n");
        return;
    }
    printf("Statistiche PG %s\n",x->pg.name);
    stampa_STAT(&x->pg.stat,stdout);
    printf("Statistiche con bonus/malus\n");
    stat_t stat_now = x->pg.stat;
    if(x->pg.equip != NULL && x->pg.equip->status == 1)
        stat_now = sum_stat_now(stat_now,x);
    stampa_STAT(&stat_now,stdout);
}
stat_t sum_stat_now (stat_t stat_now, link_nodoPg x){
    int i;
    for(i=0; i<MAX_EQUIP; i++){
        if(x->pg.equip->vettEq[i] != NULL) {
            stat_now.hp += x->pg.equip->vettEq[i]->stat.hp;
            stat_now.mp += x->pg.equip->vettEq[i]->stat.mp;
            stat_now.atk += x->pg.equip->vettEq[i]->stat.atk;
            stat_now.def += x->pg.equip->vettEq[i]->stat.def;
            stat_now.mag += x->pg.equip->vettEq[i]->stat.mag;
            stat_now.spr += x->pg.equip->vettEq[i]->stat.spr;
        }
    }
    if(stat_now.hp < 0)
        stat_now.hp = 0;
    if(stat_now.mp < 0)
        stat_now.mp = 0;
    if(stat_now.atk < 0)
        stat_now.atk = 0;
    if(stat_now.def < 0)
        stat_now.def = 0;
    if(stat_now.mag < 0)
        stat_now.mag = 0;
    if(stat_now.spr < 0)
        stat_now.spr = 0;
    return stat_now;
}

/*EQUIPAGGIAMENTO*/
void w_update_Equip (link_tabInv tabInv, link_tabPg tabPg){
    int i;
    link_nodoPg  x;
    PG_KEY id = PG_KEYget(stdin,0);
    x = linearSearchList (tabPg,id);
    if(x == NULL){
        printf("Codice personaggio non presente in lista\n");
        return;
    }
    if (x->pg.equip != NULL)
        stampaEquip (x,stdout);
    else
        x->pg.equip = init_tabEquip();
    x->pg.equip->status = 1;
    if(x->pg.equip->inUso==MAX_EQUIP){
        printf("MAX EQUIP RAGGIUNTO! ");
        i = remove_equip (x);
    }
    else
        for(i=0; i<MAX_EQUIP && x->pg.equip->vettEq[i]!=NULL; i++);
    add_equip (x,i,tabInv);
}
void add_equip (link_nodoPg x, int indice, link_tabInv tabInv){
    int i,ii;
    do{
        printf("Seleziona da INVENTARIO:\n");
        stampa_INV(tabInv,stdout);
        scanf("%d",&i);
    }while (i<0 || i>tabInv->maxInv);
    //assert(i>0 && i<=tabInv->maxInv);
    for(ii=0;ii<MAX_EQUIP; ii++)
        if(ii!=indice && x->pg.equip->vettEq[ii] != NULL && x->pg.equip->vettEq[ii] == &(tabInv->vettInv[i-1])){
            printf("NON BARARE! OGGETTO PRESENTE IN EQUIPAGGIAMENTO\n");
            return;
        }
    x->pg.equip->inUso ++;
    x->pg.equip->vettEq[indice] = &(tabInv->vettInv[i-1]);
    stampaEquip (x,stdout);
}
void w_remove_Equip (link_tabInv tabInv, link_tabPg tabPg){
    PG_KEY  id = PG_KEYget(stdin,0);
    link_nodoPg  x;
    x = linearSearchList (tabPg,id);
    if(x == NULL){
        printf("Codice personaggio non presente in lista\n");
        return;
    }
    if (x->pg.equip != NULL){
        stampaEquip(x,stdout);
        if (remove_equip(x)>=0){
            printf("RIMOSSO\n");
            stampaEquip(x,stdout);
        }
        if(x->pg.equip->inUso == 0)
            x->pg.equip =  free_tabEquip(x->pg.equip);
    }
    else
        printf("PG %s non possiede alcun equipaggiamento\n",x->pg.name);
}
int remove_equip(link_nodoPg x){
    int i;
    do{
        printf("Selezionare id_slot oggetto da rimuovere: ");
        scanf("%d", &i);
        i--;
    }while (i<0 || i> MAX_EQUIP || x->pg.equip->vettEq[i]==NULL);
    x->pg.equip->vettEq[i] = NULL;
    x->pg.equip->inUso --;
    return i;

}

/*Stampa*/
void stampa_PG(pg_t *pg,FILE *out){
    fprintf(out,"%s %s %s\n",pg->id,pg->name,pg->class);
}

void stampa_PG_LIST(link_nodoPg l,FILE *out){
    if(l==NULL)
        return;
    stampa_PG(&l->pg,out);
    if(l->pg.equip != NULL && l->pg.equip->inUso>0)
        stampaEquip(l,out);
    stampa_PG_LIST(l->next,out);
}
void stampaEquip (link_nodoPg x, FILE *out){
    int i;
    if(x->pg.equip == NULL || x->pg.equip->inUso <= 0)
        return;
    assert(x->pg.equip != NULL);
    fprintf(out,"\nEQUIPAGGIAMENTO di %s: \n",x->pg.name);
    for (int i = 0; i < MAX_EQUIP; i++)
        if(x->pg.equip->vettEq[i] != NULL){
            fprintf(out,"SLOT %d >",i+1);
            stampa_INV_i(x->pg.equip->vettEq[i],out);
        }else fprintf(out,"SLOT %d > (SLOT_VUOTO)\n",i+1);
    printf("\n");
}

