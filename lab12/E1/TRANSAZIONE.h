//
// Created by Gianluca Graziadei on 30/12/2020.
//
#ifndef E1_TRANSAZIONE_H
#define E1_TRANSAZIONE_H
#include "TIME.h"
typedef struct {
    ORA time;
    float value;
    int numero;
}TRANSAZIONE;
TRANSAZIONE TRANSAZIONE_init (FILE *fin);
TRANSAZIONE TRANSAZIONE_void();
void TRANSAZIONE_print(FILE *fout, TRANSAZIONE *tp);
#endif //E1_TRANSAZIONE_H
