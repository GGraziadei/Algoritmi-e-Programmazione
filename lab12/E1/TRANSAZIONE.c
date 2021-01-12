#include "TRANSAZIONE.h"

TRANSAZIONE TRANSAZIONE_void(){
    TRANSAZIONE t;
    t.time  = TIME_void();
    t.value = 0;
    t.numero = 0;
    return t;
}

TRANSAZIONE TRANSAZIONE_init (FILE *fin){
    assert(fin != NULL);
    TRANSAZIONE t = TRANSAZIONE_void();
    t.time = TIME_init(fin);
    fscanf(fin,"%f %d",&t.value,&t.numero);
    return t;
}
