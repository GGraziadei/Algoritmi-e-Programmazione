#ifndef E1_CORPOLIBERO_H
#define E1_CORPOLIBERO_H
#include "ELMENT.h"
#include "DIAG.h"
#include "PROGRAM.h"

#define indexVOID -100
typedef struct elementi *ELMENTS;
ELMENTS ELMENTS_init(int N);
ELMENT ELMENTS_getELMENT(ELMENTS eA,int index);
void ELMENTS_read(FILE *fp,ELMENTS eA);
void ELMENTS_free(ELMENTS eA);
void ELMENTS_print(FILE *out,ELMENTS eA);
void CORPOLIBERO_greedy(ELMENTS eA,int DD,int DP);
int CORPOLIBERO_checkPROGRAM(PROGRAM *p,int DP);
void ELMENTS_sort(ELMENTS eA);
#endif