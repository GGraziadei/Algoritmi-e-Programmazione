#ifndef E1_ELMENTSARRAY_H
#define E1_ELMENTSARRAY_H
#include "ELMENT.h"
#define indexVOID -100
typedef struct elementi *ELMENTS;
ELMENTS ELMENTS_init(int N);
void ELMENTS_read(FILE *fp,ELMENTS eA);
void ELMENTS_free(ELMENTS eA);
void ELMENTS_print(FILE *out,ELMENTS eA);
float ELMENTS_getValue(int index,ELMENTS eA);
void ELMENTS_printE(FILE *out,int indice,ELMENTS eA);
int ELMENTS_getN(ELMENTS eA);
int ELMENTS_getLevel(int index,ELMENTS eA);
#endif