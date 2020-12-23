#include <stdio.h>
#include "elmentsArray.h"
#include "diagLIST.h"
#define ELEMENTI "elementi.txt"
#define DBG 1
#define DD 10
ELMENTS leggiElementi();
void creaDiagList(DIAGLIST diag,ELMENTS  eA);
void creaDiagListR(DIAGLIST diag,ELMENTS eA,int *tmp_vector,int pos,float value,int livello);
int main() {
    ELMENTS eA = leggiElementi();
    DIAGLIST diag = DIAGLIST_init();
    #if DBG
    ELMENTS_print(stdout,eA);
    #endif
    creaDiagList(diag,eA);
    DIAGLIST_free(diag);
    ELMENTS_free(eA);
    return 0;
}
void creaDiagList(DIAGLIST diag,ELMENTS  eA){
    int *tmp_vector = malloc(DIAG_num*sizeof (int ));
    creaDiagListR(diag,eA,tmp_vector,0,0,0);
    free(tmp_vector);
}
void creaDiagListR(DIAGLIST diag,ELMENTS eA,int *tmp_vector,int pos, float value,int livello){
    int i;
    if(pos >= DIAG_num)
        DIAGLIST_add(tmp_vector,pos,value,livello,diag,eA);
    for(i=0; i<ELMENTS_getN(eA); i++){
        if(pos==0 || ( (livello + ELMENTS_getLevel(i,eA)) <= DD && ELMENTS_check(i,tmp_vector[i-1]))){
            tmp_vector[pos] = i;
            value += ELMENTS_getValue(i,eA);
            creaDiagListR(diag,eA,tmp_vector,pos+1,value,livello);
            value -= ELMENTS_getValue(i,eA);
        }else DIAGLIST_add(tmp_vector,pos,value,livello,diag,eA);
    }
}
ELMENTS leggiElementi(){
    int N;
    ELMENTS eA;
    FILE *fp = fopen(ELEMENTI,"r");
    fscanf(fp,"%d",&N);
    eA = ELMENTS_init(N);
    ELMENTS_read(fp,eA);
    fclose(fp);
    return eA;
}

