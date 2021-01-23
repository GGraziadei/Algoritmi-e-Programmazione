#ifndef E1_DIAG_H
#define E1_DIAG_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ELMENT.h"
#define DIAG_num 5
typedef struct{
    int livello,num;
    ELMENT e_inDiag[DIAG_num];
    float value;
}DIAG;

DIAG DIAG_void();
void DIAG_free(DIAG *dp);
void DIAG_cpy(DIAG *d1,DIAG *d2);
void DIAG_print(FILE *out,DIAG *dp);
void DIAG_addE(DIAG *d,ELMENT *e);
#endif //E1_DIAG_H
