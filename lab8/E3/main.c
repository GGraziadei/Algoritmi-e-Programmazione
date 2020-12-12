#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#define S 50+1
#define MAX_EQUIP 8
#define PERSONAGGI "pg.txt"
#define INVENTARIO "inventario.txt"
typedef enum {
    r_add_pg,r_delete_pg, r_update_equip,r_remove_equip,r_stat_pg,r_fine,r_errore
}e_comandi;
typedef enum {
    e_file,e_keyboard
}add_from;
typedef struct {
     int hp,mp,atk,def,mag,spr;
}stat_t;
typedef struct t_int_t{
    char *name,*type;
    stat_t stat;
}inv_t;
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

e_comandi leggiComando();
link_tabPg init_tabPg();
link_tabInv init_tabInv();
link_nodoPg new_node_PG (link_nodoPg p_next, pg_t x_pg);
link_tabPg  libera_tabPG (link_tabPg t);
link_tabInv libera_tabINV (link_tabInv t);
pg_t init_PG (char *tmp_str);
pg_t init_PG_cmd ();
inv_t init_INV (char *str);
stat_t init_stat(char *str);
stat_t statSetVOID ();
stat_t sum_stat_now(stat_t stat_now,link_nodoPg x);
void pgSetVOID(pg_t *pg);
link_tabEquip init_tabEquip();
void w_add_PG (link_tabPg,add_from);
void w_add_INV (link_tabInv tabInv, add_from source);
void w_delete_PG(link_tabPg tabPg);
void w_update_Equip (link_tabInv tabInv, link_tabPg tabPg);
void w_remove_Equip (link_tabInv tabInv, link_tabPg tabPg);
void w_stat (link_tabPg tabPg);
void add_PG (pg_t pg, link_tabPg tabPg);
void listInsTailFast(link_tabPg tabPg,pg_t pg);
int listDelID (link_tabPg  tabPg,int id);
void add_equip (link_nodoPg x, int indice, link_tabInv tabInv);
int remove_equip(link_nodoPg x);
void free_node_PG (link_nodoPg p);
void free_PG (pg_t *p_pg);
void free_INV(inv_t inv);
link_nodoPg liberaLista_PG (link_nodoPg h);
link_nodoPg linearSearchList (link_tabPg tabPg,int id);
void selezionaDati(e_comandi c,link_tabPg l1, link_tabInv l2);
void toMinuscolo (char *str);
void toMaiuscolo(char *str);
void stampa_STAT (stat_t stat);
void stampa_PG(pg_t pg);
void stampa_INV_i(inv_t inv);
void stampa_PG_LIST(link_nodoPg l);
void stampa_INV(link_tabInv tabInv);
void stampaEquip (link_nodoPg x);



int main() {
    setbuf(stdout, 0);
    e_comandi CODcomando = r_add_pg;

    link_tabPg tabPg = init_tabPg();
    link_tabInv tabInv = init_tabInv();

    w_add_PG(tabPg,e_file);
    w_add_INV(tabInv,e_file);

    while(CODcomando != r_fine){
        CODcomando = leggiComando();
        if(CODcomando != r_fine)
          selezionaDati (CODcomando,tabPg,tabInv);
    }

    tabPg = libera_tabPG (tabPg);
    tabInv = libera_tabINV (tabInv);

    return 0;
}

e_comandi leggiComando(){
    int i;
    char cmd[S];
    e_comandi c;
    char *c_tabella[r_errore]={ "add_pg","delete_pg", "update_equip","remove_equip","stat","fine" };
    char *c_tabella_Comandi[r_errore]={
            "\t1. Aggiungi personaggio: add_pg",
            "\t2. Cancella personaggio: delete_pg <code>",
            "\t3. Aggiorna equipaggiamento di un personaggio: update_equip <code>",
            "\t3. Elimina oggetto equipaggiamento di un personaggio: remove_equip <code>",
            "\t4. Statistische personaggio: stat <code>",
            "\tTermina esecuzione: fine"
    };
    do{
        printf("\t\tMENU DI SELEZIONE\n");
        for(i=0; i<r_errore; i++)
            printf("%s \n",c_tabella_Comandi[i] );
        printf("\tInserire comando come da specifiche sopra indicate: ");

        if(scanf("%s",cmd)==1){
            toMinuscolo(cmd);
            c = r_add_pg;
            while(c<r_errore && strcmp(c_tabella[c],cmd)!=0 )
                c++;
        }
        if(c==r_errore)
            printf("Comando non valido\n\n");
    }while(c == r_errore);
    return c;
}

void selezionaDati(e_comandi c,link_tabPg tabPg,link_tabInv tabInv){
/*Comandi disponibili   r_add_pg,r_delete_pg, r_update_equip,r_stat_pg,r_fine,r_errore*/
    assert(c<r_fine && c>=r_add_pg);
    switch (c) {
        case r_add_pg: w_add_PG(tabPg,e_keyboard);
            break;
        case r_delete_pg: w_delete_PG(tabPg);
            break;
        case r_update_equip: w_update_Equip(tabInv,tabPg);
            break;
        case r_stat_pg: w_stat(tabPg);
            break;
        case r_remove_equip: w_remove_Equip(tabInv,tabPg);
            break;
        default:exit(-1);
    }
}
void w_stat (link_tabPg tabPg){
    char code[S];
    int id,i;
    link_nodoPg  x;

    scanf("%s",code);
    assert(code != NULL);
    sscanf(code,"PG%d",&id);
    x = linearSearchList (tabPg,id);
    if(x == NULL){
        printf("Codice personaggio non presente in lista\n");
        return;
    }
    printf("Statistiche PG %s\n",x->pg.name);
    stampa_STAT(x->pg.stat);
    printf("Statistiche con bonus/malus\n");
    stat_t stat_now = x->pg.stat;
    if(x->pg.equip != NULL && x->pg.equip->status == 1)
        stat_now = sum_stat_now(stat_now,x);
    stampa_STAT(stat_now);
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

void w_update_Equip (link_tabInv tabInv, link_tabPg tabPg){
    char code[S];
    int id,i;
    link_nodoPg  x;

    scanf("%s",code);
    assert(code != NULL);
    sscanf(code,"PG%d",&id);
    x = linearSearchList (tabPg,id);
    if(x == NULL){
        printf("Codice personaggio non presente in lista\n");
        return;
    }
    if (x->pg.equip != NULL)
        stampaEquip (x);
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
    x->pg.equip->inUso ++;
}

void w_remove_Equip (link_tabInv tabInv, link_tabPg tabPg){
    char code[S];
    int id,i =-1;
    link_nodoPg  x;

    scanf("%s",code);
    assert(code != NULL);
    sscanf(code,"PG%d",&id);
    x = linearSearchList (tabPg,id);
    if(x == NULL){
        printf("Codice personaggio non presente in lista\n");
        return;
    }
    if (x->pg.equip != NULL){
        stampaEquip(x);
        if (remove_equip(x)>=0){
            x->pg.equip->inUso --;
            printf("RIMOSSO\n");
            stampaEquip(x);
        }
    }
    else
        printf("PG %s non possiede alcun equipaggiamento\n",x->pg.name);
}

link_tabEquip init_tabEquip(){
    int i;
    link_tabEquip tab = malloc(sizeof(struct tabEquip_t));
    tab->status = tab->inUso = 0;
    for (int i = 0; i < MAX_EQUIP; ++i)
        tab->vettEq[i] = NULL;
    return tab;
}

void add_equip (link_nodoPg x, int indice, link_tabInv tabInv){
    int i,ii;
    printf("Seleziona da INVENTARIO:\n");
    stampa_INV(tabInv);
    scanf("%d",&i);
    if(i<0 && i>tabInv->maxInv)
        return;
    //assert(i>0 && i<=tabInv->maxInv);
    for(ii=0;ii<MAX_EQUIP; ii++)
        if(ii!=indice && x->pg.equip->vettEq[ii] != NULL && x->pg.equip->vettEq[ii] == &(tabInv->vettInv[i-1])){
            printf("NON BARARE! OGGETTO PRESENTE IN EQUIPAGGIAMENTO\n");
            return;
        }
    x->pg.equip->vettEq[indice] = &(tabInv->vettInv[i-1]);
    stampaEquip (x);
}
int remove_equip(link_nodoPg x){
    int i;
    printf("Selezionare id_slot oggetto da rimuovere: ");
    scanf("%d", &i);
    i--;
    if(x->pg.equip->vettEq[i] != NULL) {
        x->pg.equip->vettEq[i] = NULL;
        return i;
    }
    else
        printf("Oggetto non presente in equipaggiamento\n");
    return -1;
}
void stampaEquip (link_nodoPg x){
    int i;
    assert(x->pg.equip != NULL);
    printf("\nEQUIPAGGIAMENTO di %s: \n",x->pg.name);
    for (int i = 0; i < MAX_EQUIP; i++)
        if(x->pg.equip->vettEq[i] != NULL){
            printf("%d ->",i+1);
            stampa_INV_i(*(x->pg.equip->vettEq[i]));
        }
    printf("\n");
}

void w_add_PG (link_tabPg tabPg, add_from source){
    FILE *in;
    char tmp_str[3 * S];
    if(source == e_file) {
        in = fopen(PERSONAGGI, "r");
        assert(in != NULL);
        while (fgets(tmp_str, 3 * S, in) != NULL)
            add_PG(init_PG(tmp_str),tabPg);
        fclose(in);
    }
    else if(source == e_keyboard)
        add_PG(init_PG_cmd(),tabPg);
    printf("LISTA PERSONAGGI\n");
    stampa_PG_LIST(tabPg->headPg);
}

void add_PG (pg_t pg, link_tabPg tabPg){
    listInsTailFast(tabPg,pg);
    tabPg->nPg++;
}

void w_delete_PG(link_tabPg tabPg){
    char code[S];
    int id;
    scanf("%s",code);
    assert(code != NULL);
    sscanf(code,"PG%d",&id);
    if(listDelID(tabPg,id) == 0)
        printf("NESSUN PG eliminato.\nINSERIRE CODICE VALIDO!\n");
    printf("LISTA AGGIORNATA\n");
    stampa_PG_LIST(tabPg->headPg);

}

void listInsTailFast(link_tabPg tabPg,pg_t pg){
    if(tabPg->headPg == NULL)
        tabPg->headPg = tabPg->tailPg = new_node_PG(tabPg->tailPg,pg);
    else{
        tabPg->tailPg->next = new_node_PG(NULL,pg);
        tabPg->tailPg = tabPg->tailPg->next;
    }
}
link_nodoPg linearSearchList (link_tabPg tabPg,int id){
    link_nodoPg x=tabPg->headPg;
    while (x != NULL && x->pg.id != id )
        x = x->next;
    return x;
}
int listDelID (link_tabPg  tabPg,int id){
    link_nodoPg x,p;
    if(tabPg->headPg == NULL)
        return 0;
    for (x = tabPg->headPg, p=NULL; x!=NULL; p=x, x = x->next)
        if(id == x->pg.id){
            printf("DELETE: ");
            stampa_PG(x->pg);
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

link_tabPg init_tabPg(){
    link_tabPg tabPg = (link_tabPg) malloc(sizeof(struct tabPg_t));
    assert (tabPg != NULL);
    tabPg->headPg = tabPg->tailPg = NULL; /*LISTA VUOTA*/
    tabPg->nPg = 0;
    return tabPg;
}
void stampa_PG_LIST(link_nodoPg l){
    if(l==NULL)
        return;
    stampa_PG(l->pg);
    stampa_PG_LIST(l->next);
}
link_tabInv init_tabInv(){
    link_tabInv tabInv = (link_tabInv) malloc(sizeof(struct tabInv_t));
    assert (tabInv != NULL);
    tabInv->vettInv = NULL;
    tabInv->maxInv = tabInv->nInv = 0;
    return tabInv;
}
pg_t init_PG (char *tmp_str){
    char nome[S],class[S];
    int i;
    pg_t pg_x;
    pgSetVOID(&pg_x);

    sscanf(tmp_str,"PG%d %s %s%n",&pg_x.id,nome,class,&i);
    pg_x.name = strdup(nome);
    pg_x.class = strdup(class);
    assert(pg_x.name != NULL && pg_x.class != NULL);

    pg_x.stat = init_stat(&(tmp_str[i]));

    pg_x.equip = NULL;
    return pg_x;
}
void pgSetVOID(pg_t *pg){
    pg->stat = statSetVOID();
    pg->name = pg->class = NULL;
    pg->equip = NULL;
    pg->id = -1;
}
pg_t init_PG_cmd (){
    int l;
    char nome[S],classe[S];
    pg_t pg_x;
    do{
        fflush(stdin);
        printf("Inserire CODICE (PGXXXX): ");
        gets(nome);
        l = strlen(nome);
        if (nome[0]>'A')
            toMaiuscolo(nome);
    }while(l!=6 || strncmp(nome,"PG",2));
    sscanf(nome,"PG%04d",&pg_x.id);

    printf("Inserire NOME: ");
    gets(nome);
    printf("Inserire CLASSE: ");
    gets(classe);
    pg_x.name = strdup(nome);
    pg_x.class = strdup(classe);
    assert(pg_x.class != NULL && pg_x.name != NULL);

    printf("Inserire STAT (hp mp atk def mag spr, separati da spazio): ");
    gets(nome);
    pg_x.stat = init_stat(nome);
    pg_x.equip = NULL;

    return pg_x;
}
void stampa_PG(pg_t pg){
    printf("PG%04d  %s %s\n",pg.id,pg.name,pg.class);
}

stat_t init_stat(char *str){
    stat_t stat_x = statSetVOID();
    assert(sscanf(str,"%d %d %d %d %d %d",&stat_x.hp,&stat_x.mp,
                  &stat_x.atk,&stat_x.def,&stat_x.mag,&stat_x.spr)==6);
    return stat_x;
}
stat_t statSetVOID (){
    stat_t stat;
    stat.hp = stat.mp = stat.atk = stat.def = stat.mag = stat.spr = 0;
    return stat;
}
void  stampa_STAT (stat_t stat){
    printf("hp: %d\nmp: %d\natk: %d\ndef: %d\nmag: %d\nspr: %d\n\n",stat.hp,stat.mp,stat.atk,stat.def,stat.mag,stat.spr);
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
void free_PG (pg_t *p_pg){
    free(p_pg->class);
    free(p_pg->name);
    if(p_pg->equip != NULL)free(p_pg->equip);
}

link_nodoPg liberaLista_PG (link_nodoPg h){
    if(h->next == NULL)
        return NULL;
    h->next = liberaLista_PG(h->next);
    free_PG(&(h->pg));
    free_node_PG(h);
    return NULL;
}
link_tabPg  libera_tabPG (link_tabPg t){
    t->tailPg = t->headPg = liberaLista_PG(t->headPg);
    t->tailPg = 0;
    return NULL;
}

void w_add_INV (link_tabInv tabInv, add_from source){
    FILE *in;
    char tmp_str[3 * S];
    if(source == e_file) {
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
    printf("\nINVENTARIO: \n");
    stampa_INV(tabInv);
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
link_tabInv libera_tabINV (link_tabInv t){
    int i;
    for(i=0; i<t->maxInv; i++)
        free_INV(t->vettInv[i]);
    free(t->vettInv);
    return NULL;
}
void stampa_INV(link_tabInv tabInv){
    int i;
    for (int i = 0; i < tabInv->maxInv; ++i){
        printf("%d ->",i+1);
        stampa_INV_i(tabInv->vettInv[i]);
    }
}
void stampa_INV_i(inv_t inv){
    printf("%s %s\n",inv.name,inv.type);
}
void toMinuscolo (char *str){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]<'a')
            str[i] = tolower(str[i]);
}
void toMaiuscolo(char *str){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]>='a')
            str[i] = toupper(str[i]);
}