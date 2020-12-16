#include "inv.h"

void stat_void (stat_t *statp,int init_value){
    statp->hp = statp->mp = statp->atk = statp->def = statp->mag = statp->spr = init_value;
}

void stat_read(FILE *fp, stat_t *statp){
    assert(fp != NULL);
    stat_void(statp,0);
    assert(fscanf(fp,"%d %d %d %d %d %d",&statp->hp,&statp->mp,
                  &statp->atk,&statp->def,&statp->mag,&statp->spr) == 6);
}

void stat_print(FILE *fp, stat_t *statp, int soglia){
    /*Controllo soglia: funzione inferiormente limitata, se limite inferiore è -infinito (INT_MIN)
     * allora qualsiasi valore è ammissibile, utile per la inv_print*/
    fprintf(fp,"hp: %d\nmp: %d\natk: %d\ndef: %d\nmag: %d\nspr: %d\n",
            statp->hp>soglia?statp->hp:soglia,
            statp->mp>soglia?statp->mp:soglia,
            statp->atk>soglia?statp->atk:soglia,
            statp->def>soglia?statp->def:soglia,
            statp->mag>soglia?statp->mag:soglia,
            statp->spr>soglia?statp->spr:soglia);
}

stat_t stat_sum (stat_t *s1, stat_t *s2){
    stat_t s;
    stat_void(&s,0);
    s.hp = s1->hp+s2->hp;
    s.mp = s1->mp+s2->mp;
    s.atk = s1->atk+s2->atk;
    s.def = s1->def+s2->def;
    s.mag = s1->mag+s2->mag;
    s.spr = s1->spr+s2->spr;
    return s;
}
stat_t stat_min (stat_t *s1, stat_t *s2){
    stat_t s;
    stat_void(&s,0);
    s.hp = s1->hp-s2->hp;
    s.mp = s1->mp-s2->mp;
    s.atk = s1->atk-s2->atk;
    s.def = s1->def-s2->def;
    s.mag = s1->mag-s2->mag;
    s.spr = s1->spr-s2->spr;
    return s;
}
int check_stat (stat_t *s,int soglia){
    if(s->hp < soglia || s->mp < soglia ||s->atk < soglia ||s->def < soglia
                ||s->mag < soglia ||s->spr < soglia)
        return 0;
    return 1;
}
void inv_void(inv_t *invp, int init_value){
    invp->nome[0] = invp->tipo[0] = init_value;
    stat_void(&invp->stat,0);
}

int inv_read(FILE *fp, inv_t *invp){
    assert(fp != NULL && !feof(fp));
    inv_void (invp,0);
    assert ( fscanf(fp,"%s %s",invp->nome,invp->tipo) == 2);
    stat_read(fp,&invp->stat);
    return 1;
}
void inv_print(FILE *fp, inv_t *invp,mod_inv stampaMode){
   if(stampaMode >= mode_inv){
       fprintf(fp,"NOME: %s TIPO: %s\n",invp->nome,invp->tipo);
       if(stampaMode == mode_inv_stat)
           stat_print(fp,&invp->stat,INT_MIN);
   }
}
INV_KEY inv_getKey(inv_t *invp){
    return invp->nome;
}
int INV_KEY_eq (INV_KEY k1, INV_KEY k2){
    return strcmp(k1,k2)!=0?0:1;
}
stat_t inv_getStat(inv_t *invp){
    return invp->stat;
}