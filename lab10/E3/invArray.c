#include "invArray.h"

struct invArray_s{
    inv_t *vettInv;
    int nInv,maxInv;
};

invArray_t invArray_init(){
    invArray_t invArray = (invArray_t) malloc(sizeof (struct invArray_s));
    invArray->nInv = 0;
    invArray->vettInv = NULL;
    return invArray;
}
void invArray_read(FILE *fp, invArray_t invArray){
    assert(fp != NULL);
    fscanf(fp,"%d",&invArray->nInv);
    invArray->maxInv = invArray->nInv;
    invArray->vettInv = (inv_t *) malloc(invArray->nInv * sizeof (inv_t));
    invArray->nInv = 0;
    while (invArray->nInv < invArray->maxInv &&
            inv_read(fp,&invArray->vettInv[invArray->nInv++]));
}

void invArray_free(invArray_t invArray){
    free(invArray->vettInv);
    free(invArray);
}
void invArray_print(FILE *fp, invArray_t invArray){
    int i;
    for(i=0; i<invArray->nInv; i++) {
        fprintf(fp,"%2d >",i);
        inv_print(fp, invArray_getByIndex(invArray, i), mode_inv);
    }
}
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    if(index >= 0 && index< invArray->maxInv)
        inv_print(fp,invArray_getByIndex(invArray,index),mode_inv);
    else fprintf(fp,"INDICE %d non presente in struttura dati\n",index);
}

inv_t *invArray_getByIndex(invArray_t invArray, int index){
    return &invArray->vettInv[index];
}
int invArray_searchByName(invArray_t invArray, INV_KEY name){
    int i;
    for(i=0; i<invArray->nInv; i++)
        if(INV_KEY_eq(name,inv_getKey(invArray_getByIndex(invArray,i))))
            return i;
    return -1;
}

