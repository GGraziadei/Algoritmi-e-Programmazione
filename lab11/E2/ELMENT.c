#include "ELMENT.h"
char buf[S];
ELMENT ELMENT_init(){
    ELMENT e;
    e.nome = NULL;
    e.livello = 0;
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
void ELMENT_free(ELMENT e){
    free(e.nome);
}
void ELMENT_print(FILE *out, ELMENT *e){
    fprintf(out,"%s %d %d %d %d %d %f %d\n",e->nome,e->parametri_tecnici[p_tipologia],e->parametri_tecnici[p_d_entrata],
            e->parametri_tecnici[p_d_uscita],e->parametri_tecnici[p_precedenza],e->parametri_tecnici[p_finale],
            e->punti,e->livello);
}
