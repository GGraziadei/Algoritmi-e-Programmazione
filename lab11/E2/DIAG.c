#include "DIAG.h"


DIAG DIAG_void(){
    int i;
    DIAG d;
    d.livello  = 0;
    d.value = 0;
    d.num = 0;
    for(i=0; i<DIAG_num; i++)
        d.e_inDiag[i] = ELMENT_init();
    return d;
}

void DIAG_free(DIAG *dp){
    int i;
    for(i=0; i<DIAG_num; i++)
        ELMENT_free(dp->e_inDiag[i]);
}


void DIAG_print(FILE *out,DIAG *dp){
    int i;
    fprintf(out,"\nDIAG\nPunteggio: %0.2f\tElementi:%d\nLivello difficolta': %d\n",dp->value,dp->num,dp->livello);
    for(i=0; i<dp->num; i++)
        fprintf(out,"%s\t",dp->e_inDiag[i].nome);
}

void DIAG_cpy(DIAG *d1,DIAG *d2){
    int i;
    if(d1->num > 0)
        DIAG_free(d1);
    *d1 = DIAG_void();
    d1->num = d2->num;
    d1->livello = d2->livello;
    d1->value = d2->value;
    for(i=0; i<d1->num; i++)
        ELMENT_cpy(&d1->e_inDiag[i],&d2->e_inDiag[i]);
}
void DIAG_addE(DIAG *d,ELMENT *e){
    ELMENT_cpy(&d->e_inDiag[d->num++],e);
    d->livello += e->livello;
    d->value += e->punti;
}