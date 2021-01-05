#include "elmentsArray.h"
struct elementi{
    int N;
    ELMENT *elementi;
};
ELMENTS ELMENTS_init(int N){
    ELMENTS eA = malloc(sizeof (struct elementi));
    eA->elementi = malloc(N*sizeof (ELMENT));
    eA->N = N;
    return eA;
}
void ELMENTS_read(FILE *fp,ELMENTS eA){
    assert(fp != NULL);
    int N = eA->N;
    eA->N = 0;
    while (eA->N < N){
        eA->elementi[eA->N] = ELMENT_init();
        ELMENT_read(fp,&eA->elementi[eA->N++]);
    }
    assert(eA->N == N);
}
void ELMENTS_print(FILE *out,ELMENTS eA){
    int i;
    for(i=0; i< eA->N; i++)
        ELMENTS_printE(out,i,eA);
}
void ELMENTS_printE(FILE *out,int indice,ELMENTS eA){
    if(indice >= 0 && indice<eA->N)
        ELMENT_print(out,&eA->elementi[indice]);
}
void ELMENTS_free(ELMENTS eA){
    int i;
    for(i=0; i<eA->N; i++)
        ELMENT_free(eA->elementi[i]);
    free(eA);
}
float ELMENTS_getValue(int index,ELMENTS eA){
    return eA->elementi[index].punti;
}
int ELMENTS_getLevel(int index,ELMENTS eA){
    return eA->elementi[index].livello;
}
int ELMENTS_getN(ELMENTS eA) {
    return eA->N;
}
int ELMENTS_check(int indiceA,int indiceB,float value){

}