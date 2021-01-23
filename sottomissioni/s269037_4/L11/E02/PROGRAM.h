#ifndef E1_PROGRAM_H_H
#define E1_PROGRAM_H_H
#include "DIAG.h"

#define DIAG_IN_PROGRAM 3
typedef struct {
    DIAG programmaGara[DIAG_IN_PROGRAM];
    float value,bonus;
    int livello;
}PROGRAM;
PROGRAM PROGRAM_void();
void PROGRAM_print(FILE *out,PROGRAM *pp);
void PROGRAM_cpy(PROGRAM *p1,PROGRAM*p2);
void PROGRAM_free(PROGRAM *p);
#endif //E1_PROGRAM_H_H
