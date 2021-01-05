#ifndef E1_TITOLO_H
#define E1_TITOLO_H
#include "quotazioniBST.h"
typedef struct titolo *TITOLO;
typedef char* TITOLO_KEY;
typedef enum {
    e_titolo,e_titolo_bst
}e_pint;
TITOLO_KEY TITOLO_extractKey(TITOLO stock);
TITOLO TITOLO_init(TITOLO_KEY cod);
void TITOLO_free(TITOLO stock);
void TITOLO_addTransazioni(FILE *fin,TITOLO stock);
void TITOLO_storage (FILE *fout,TITOLO stock,e_pint print_e);
TITOLO TITOLO_NULL();
void TITOLO_UI(TITOLO stock);
#endif //E1_TITOLO_H
