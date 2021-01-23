#include "ELMENT.h"
char buf[S];

ELMENT ELMENT_init(){
    ELMENT e;
    e.nome = NULL;
    e.livello = -100;
    e.punti  = 0;
    return e;
}
void ELMENT_read(FILE *fp,ELMENT *e){
    assert(fp!= NULL);
    fscanf(fp,"%s",buf);
    e->nome = strdup(buf);
    assert ( fscanf(fp,"%d %d %d %d %d %f %d",&e->parametri_tecnici[p_tipologia],&e->parametri_tecnici[p_d_entrata],
                    &e->parametri_tecnici[p_d_uscita],&e->parametri_tecnici[p_precedenza],&e->parametri_tecnici[p_finale],
                    &e->punti,&e->livello) == (p_void+2) );
}
void ELMENT_cpy(ELMENT *e1,ELMENT *e2){
    int i;
    e1->app = e2->app;
    e1->livello  = e2->livello;
    e1->punti  = e2->punti;
    e1->nome  = strdup(e2->nome);
    for(i=0; i<p_void; i++)
        e1->parametri_tecnici[i] = e2->parametri_tecnici[i];
}
void ELMENT_free(ELMENT e){
    free(e.nome);
}
void ELMENT_print(FILE *out, ELMENT *e){
    fprintf(out,"%s %d %d %d %d %d %f %d APP: %f\n",e->nome,e->parametri_tecnici[p_tipologia],e->parametri_tecnici[p_d_entrata],
            e->parametri_tecnici[p_d_uscita],e->parametri_tecnici[p_precedenza],e->parametri_tecnici[p_finale],
            e->punti,e->livello,e->app);
}
