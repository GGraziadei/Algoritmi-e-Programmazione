#ifndef E1_DIAG_H
#define E1_DIAG_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DIAG_num 5
typedef struct{
    int *elments_index,num,livello;
    float value;
}DIAG;
DIAG DIAG_init(int N, int *index_vector,float value,int livello);
DIAG DIAG_void();
int DIAG_eq(DIAG *d1,DIAG *d2);
void DIAG_free(DIAG *dp);
void DIAG_cpy(DIAG *d1,DIAG *d2);
void DIAG_print(FILE *out,DIAG *dp);
#endif //E1_DIAG_H
