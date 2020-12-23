#include "pg.h"

void pg_void (pg_t *pgp, int init_value){
    pgp->classe[0] = pgp->nome[0] = pgp->cod[0] = init_value ;
    pgp->equip  = equipArray_init();
    stat_void(&pgp->b_stat,init_value);
    stat_void(&pgp->eq_stat,init_value);
}
int pg_read(FILE *fp, pg_t *pgp){
    assert(fp != NULL);
    pg_void(pgp,0);
    if ( fscanf(fp,"%s %s %s",pgp->cod,pgp->nome,pgp->classe) == 3 ) {
        stat_read(fp, &(pgp->b_stat));
        pgp->eq_stat = pgp->b_stat;
        if(strncmp("PG",pgp->cod,2)==0)
            return 1;
    }
    return 0;
}

void pg_clean(pg_t *pgp){
    if(pgp->equip != NULL)
        equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray, mod_pg stampaMode){
    if(stampaMode >= mode_pg) {
        fprintf(fp, "CODICE %s NOME : %s CLASSE: %s\n", pgp->cod, pgp->nome, pgp->classe);
        if ( stampaMode >= mode_pg_equip) {
            fprintf(fp, "EQUIPAGGIAMENTO:\n");
            if(pgp->equip != NULL )
                equipArray_print(fp, pgp->equip, invArray);
            if (stampaMode >= mode_pg_stat) {
                fprintf(fp, "STATISTICHE BASE:\n");
                stat_print(fp, &pgp->b_stat, INT_MIN);
                if(stampaMode == mode_pg_equip_stat) {
                    fprintf(fp, "STATISTICHE CON BONUS/MALUS EQUIP (soglia: %d):\n", SOGLIA);
                    stat_print(fp, &pgp->eq_stat, SOGLIA);
                }
            }
        }
    }
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray){
    stat_t delta_stat;
    equipArray_t delta_equip;
    fprintf(stdout,"EQUIPAGGIAMENTO ATTUALE (di %s)\n",pgp->nome);
    equipArray_print(stdout,pgp->equip,invArray);
    delta_equip  = equipArray_init();
    equipArray_cpy (delta_equip,pgp->equip);
    delta_stat = equipArray_update(delta_equip,invArray);
    delta_stat = stat_sum(&delta_stat,&pgp->eq_stat);
    if (check_stat(&delta_stat,SOGLIA)){
        equipArray_free(pgp->equip);
        pgp->equip = delta_equip;
        pgp->eq_stat = delta_stat;
    }else{
        equipArray_free(delta_equip);
        fprintf(stdout,"Update Equip non compatibile con SOGLIA\n");
    }
    fprintf(stdout,"EQUIPAGGIAMENTO ATTUALE (di %s)\n",pgp->nome);
    equipArray_print(stdout,pgp->equip,invArray);
}
PG_KEY pg_getKey(pg_t *pgp){
    return  pgp->cod;
}
int PG_KEY_eq (PG_KEY k1, PG_KEY k2){
    return (1 - strcmp(k1,k2));
}