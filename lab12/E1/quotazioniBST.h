#ifndef E1_QUOTAZIONIBST_H
#define E1_QUOTAZIONIBST_H
#include "QUOTAZIONE.h"
#include <float.h>
#define S_soglia_bilanciamento 1
typedef struct quotazioniBST *QUOTAZIONI;
typedef enum {inorder,postorder,preorder,none}print_m;
QUOTAZIONI BST_init();
void BST_free(QUOTAZIONI bst);
QUOTAZIONE *QUOTAZIONI_search(QUOTAZIONI bst,DATA day);
int QUOTAZIONI_add (QUOTAZIONI bst,FILE *fin,int N);
void QUOTAZIONI_balance(QUOTAZIONI bst);
void BST_storage(FILE *fout,QUOTAZIONI  bst);
float QUOTAZIONI_max(QUOTAZIONI bst);
float QUOTAZIONI_min(QUOTAZIONI bst);
DATA QUOTAZIONI_minD(QUOTAZIONI bst);
DATA QUOTAZIONI_maxD(QUOTAZIONI bst);
int QUOTAZIONI_qDayNum(QUOTAZIONI bst);
void QUOTAZIONI_bestQ(float  *bestMax,float *bestMin,DATA d1,DATA d2,QUOTAZIONI bst);
#endif //E1_QUOTAZIONIBST_H