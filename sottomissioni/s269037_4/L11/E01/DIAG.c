#include "DIAG.h"

DIAG DIAG_init(int N, int *index_vector,float value,int livello){
    DIAG d;
    int i;
    d.elments_index = malloc(N*sizeof (int));
    d.value = value;
    d.num = N;
    d.livello = livello;
    for(i=0; i<N; i++)
        d.elments_index[i] = index_vector[i];
    return d;
}
DIAG DIAG_void(){
    DIAG d;
    d.livello = d.num = 0;
    d.value = 0;
    d.elments_index = calloc(DIAG_num,sizeof (int));
    return d;
}

void DIAG_free(DIAG *dp){
    free(dp->elments_index);
}

int DIAG_eq(DIAG *d1,DIAG *d2){
    int i;
    if(d1->num == d2->num ){
        for(i=0; i<d1->num; i++)
            if(d1->elments_index[i] != d2->elments_index[i])
                return 0;
        return 1;
    }return 0;
}
void DIAG_print(FILE *out,DIAG *dp){
    int i;
    fprintf(out,"\nDIAG\nPunteggio: %0.2f\tElementi:%d\nLivello difficolta': %d\n",dp->value,dp->num,dp->livello);
    for(i=0; i<dp->num; i++)
        fprintf(out,"%d\t",dp->elments_index[i]);
}

void DIAG_cpy(DIAG *d1,DIAG *d2){
    int i;
    d1->elments_index = malloc(d2->num*sizeof (int));
    d1->num = d2->num;
    d1->livello = d2->livello;
    d1->value = d2->value;
    for(i=0; i<d1->num; i++){
        d1->elments_index[i] = d2->elments_index[i];
    }
}