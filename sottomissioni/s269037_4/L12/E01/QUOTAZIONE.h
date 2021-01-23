#ifndef E1_QUOTAZIONE_H
#define E1_QUOTAZIONE_H
#include "TRANSAZIONE.h"
typedef struct {
    DATA day;
    int n_tr;
    float quotazione_day,q_sum;
}QUOTAZIONE; /*Quotazione giornaliera (ITEM del BST)*/
QUOTAZIONE *QUOTAZIONE_void();
QUOTAZIONE *QUOTAZIONE_init(DATA day);
float QUOTAZIONE_qDay(QUOTAZIONE *qp);
void QUOTAZIONE_free(QUOTAZIONE *qp);
DATA QUOTAZIONE_extractDAY(QUOTAZIONE *qp);
void QUOTAZIONE_addTRANSAZIONE(FILE *fin,QUOTAZIONE *qp);
void QUOTAZIONE_print(FILE *fout,QUOTAZIONE *qp);
#endif //E1_QUOTAZIONE_H
