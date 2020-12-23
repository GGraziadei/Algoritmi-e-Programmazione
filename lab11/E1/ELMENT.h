#ifndef E1_ELMENT_H
#define E1_ELMENT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define S 100+1
typedef char* KEY_elment;
typedef enum {
    p_tipologia,p_d_entrata,p_d_uscita,p_precedenza,p_finale,p_void
}param_tecnici;
typedef struct{
    KEY_elment nome;
    int parametri_tecnici[p_void];
    int livello;
    float punti;
}ELMENT;
ELMENT ELMENT_init();
void ELMENT_read(FILE *fp,ELMENT *e);
KEY_elment ELMENT_getKey(ELMENT *e);
void ELMENT_free(ELMENT e);
void ELMENT_print(FILE *out, ELMENT *e);
#endif //E1_ELMENT_H
