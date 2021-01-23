#ifndef E1_TITOLILIST_H
#define E1_TITOLILIST_H
#include "TITOLO.h"
typedef struct titoliLIST *TITOLI;
TITOLI TITOLI_init();
void TITOLI_free(TITOLI stockList);
void TITOLI_add(FILE *fin,TITOLI stockList);
void TITOLI_UI(TITOLI stockList,TITOLO_KEY key);
void TITOLI_print(FILE *fout,TITOLI stockList);
#endif //E1_TITOLILIST_H
