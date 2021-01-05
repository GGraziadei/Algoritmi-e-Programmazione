#ifndef E1_DIAGLIST_H
#define E1_DIAGLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "DIAG.h"
#include "PROGRAM.h"
#include "CorpoLibero.h"
typedef struct elementi *ELMENTS;
typedef struct diagList *DIAGLIST;
DIAGLIST DIAGLIST_init();
void DIAGLIST_add(DIAG d, DIAGLIST dl);
void DIAGLIST_free(DIAGLIST dl);
int DIAGLIST_getNDiag(DIAGLIST dl);
void DIAGLIST_print(FILE *out,DIAGLIST dl);
int DIAGLIST_isSaved (DIAGLIST dl, DIAG *dp);
PROGRAM DIAGLIST_makePROGRAM (DIAGLIST dl,int DP,ELMENTS eA);
int DIAGLIST_checkPROGRAM(PROGRAM *p,ELMENTS eA,int DP);
#endif //E1_DIAGLIST_H
