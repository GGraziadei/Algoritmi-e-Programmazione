#ifndef E1_CORPOLIBERO_H
#define E1_CORPOLIBERO_H
#include "ELMENT.h"
#include "diagLIST.h"
#define indexVOID -100
typedef struct elementi *ELMENTS;
ELMENTS ELMENTS_init(int N);
ELMENT ELMENTS_getELMENT(ELMENTS eA,int index);
void ELMENTS_read(FILE *fp,ELMENTS eA);
void ELMENTS_free(ELMENTS eA);
void ELMENTS_print(FILE *out,ELMENTS eA);
void CORPOLIBERO_creaDiagList(ELMENTS eA,int DD);
void CORPOLIBERO_creaPROGRAM(ELMENTS eA,int DP);

#endif