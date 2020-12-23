#ifndef E1_DIAGLIST_H
#define E1_DIAGLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "elmentsArray.h"
#define DIAG_num 5
typedef struct{
    int *elments_index,num,livello;
    float value;
}DIAG;
typedef struct diagList *DIAGLIST;
DIAG DIAG_init(int N, int *index_vector,float value,int livello);
DIAGLIST DIAGLIST_init();
void DIAGLIST_add(int *index_vector,int N,float value,int livello, DIAGLIST dl,ELMENTS eA);
void DIAGLIST_free(DIAGLIST dl);
#endif //E1_DIAGLIST_H
