#include "QUOTAZIONE.h"
static void getQUOTATION(QUOTAZIONE *qp);

QUOTAZIONE *QUOTAZIONE_void(){
    QUOTAZIONE *q = malloc(sizeof (QUOTAZIONE));
    q->transazioni = malloc(sizeof (TRANSAZIONE));
    q->transazioni[0] = TRANSAZIONE_void();
    q->n_tr = 0;
    q->quotazione_day = -1;
    q->n_tr_max = 1;
    q->day = DAY_void();
    return q;
}
QUOTAZIONE *QUOTAZIONE_init(DATA day){
    QUOTAZIONE *q = QUOTAZIONE_void();
    q->day = day;
    return q;
}
void QUOTAZIONE_free(QUOTAZIONE *qp){
    free(qp->transazioni);
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
    if(qp->n_tr == qp->n_tr_max ){
        qp->n_tr_max = 2*qp->n_tr_max;
        qp->transazioni = realloc(qp->transazioni,qp->n_tr_max*sizeof(TRANSAZIONE));
    }
    qp->transazioni[qp->n_tr++] = TRANSAZIONE_init(fin);
    getQUOTATION(qp);
}
static void getQUOTATION(QUOTAZIONE *qp){
    int i,n_op = 0;
    qp->quotazione_day = 0;
    for(i=0; i<qp->n_tr; i++) {
        qp->quotazione_day += (qp->transazioni[i].value * (float) qp->transazioni[i].numero);
        n_op += qp->transazioni[i].numero;
    }
    qp->quotazione_day /= (float) n_op;
}

void QUOTAZIONE_print(FILE *fout,QUOTAZIONE *qp){
    int i;
    DAY_print(fout,qp->day);
    fprintf(fout,"Quotazione giornaliera > %.3f\n",qp->quotazione_day);
    for(i=0; i<qp->n_tr; i++){
        fprintf(fout,"\t> ");
        TRANSAZIONE_print(fout,&qp->transazioni[i]);
    }
}