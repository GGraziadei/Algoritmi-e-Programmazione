#include "QUOTAZIONE.h"
static void getQUOTATION(QUOTAZIONE *qp);

QUOTAZIONE *QUOTAZIONE_void(){
    QUOTAZIONE *q = malloc(sizeof (QUOTAZIONE));
    q->n_tr = 0;
    q->quotazione_day = q->q_sum = 0;
    q->day = DAY_void();
    return q;
}
QUOTAZIONE *QUOTAZIONE_init(DATA day){
    QUOTAZIONE *q = QUOTAZIONE_void();
    q->day = day;
    return q;
}
void QUOTAZIONE_free(QUOTAZIONE *qp){
    free(qp);
}
DATA QUOTAZIONE_extractDAY(QUOTAZIONE *qp){
    return qp->day;
}
float QUOTAZIONE_qDay(QUOTAZIONE *qp){
    return qp->quotazione_day;
}
void QUOTAZIONE_addTRANSAZIONE(FILE *fin,QUOTAZIONE *qp){
    assert(qp != NULL);
    TRANSAZIONE t = TRANSAZIONE_init(fin);
    qp->q_sum += (t.value*(float )t.numero);
    qp->n_tr +=t.numero;
    qp->quotazione_day = qp->q_sum / (float )qp->n_tr;
}

void QUOTAZIONE_print(FILE *fout,QUOTAZIONE *qp){
    int i;
    DAY_print(fout,qp->day);
    fprintf(fout,"Quotazione giornaliera > %.3f\n",qp->quotazione_day);
}