#include "TITOLO.h"
#define N_SCELTE 6
struct titolo{
TITOLO_KEY cod;
QUOTAZIONI quotazioniBST;
int num_transazioni;
};
static char *scelte[] = {
        "Menu principale",
        "Stampa informazioni TITOLO",
        "Ricerca Quotazione giornaliera",
        "Ricerca Quotazione massima e minima in intervallo di date",
        "Ricerca Quotazione massima e minima globale",
        "Ottimizza prestazioni (Bilanciamento BST)"
};
static int stampaMenu(){
    int i = 0;
    for(i=0; i<N_SCELTE; i++)
        printf("%d > %s\n",i,scelte[i]);
    do {
        scanf("%d", &i);
    }while(i<0 && i>N_SCELTE);
    return i;
}
TITOLO TITOLO_init(TITOLO_KEY cod){
    TITOLO stock = malloc(sizeof (struct titolo));
    stock->cod = strdup(cod);
    assert(stock->cod!=NULL);
    stock->quotazioniBST = BST_init();
    stock->num_transazioni = 0;
    return stock;
}
TITOLO TITOLO_NULL(){
    return NULL;
}
void TITOLO_free(TITOLO stock){
    free(stock->cod);
    BST_free(stock->quotazioniBST);
    free(stock);
}

TITOLO_KEY TITOLO_extractKey(TITOLO stock){
    return stock->cod;
}

void TITOLO_addTransazioni(FILE *fin,TITOLO stock){
    int N;
    fscanf(fin,"%d",&N);
    assert(QUOTAZIONI_add(stock->quotazioniBST,fin,N) == N);
    stock->num_transazioni += N;
}
void TITOLO_storage (FILE *fout,TITOLO stock, e_pint print_e){
    fprintf(fout,"TITOLO: %s [transazioni in memoria %d] \tmaxQ : %.03f minQ: %.03f\n",
            TITOLO_extractKey(stock),stock->num_transazioni,QUOTAZIONI_max(stock->quotazioniBST),
                QUOTAZIONI_min(stock->quotazioniBST));
    if(print_e == e_titolo_bst)
        BST_storage(fout,stock->quotazioniBST);
}
void TITOLO_UI(TITOLO stock){
    int selezione,fineUI = 0;
    float max,min;
    DATA d1,d2;
    do {
        fineUI = 0;
        selezione = stampaMenu();
        switch (selezione) {
            case 0:fineUI=1;
                break;
            case 1:TITOLO_storage(stdout,stock,e_titolo_bst);
                break;
            case 2:{
                printf("Inserire data valida: ");
                d1 = DAY_init(stdin);
                QUOTAZIONE *q = QUOTAZIONI_search(stock->quotazioniBST,d1);
                if(q != NULL) QUOTAZIONE_print(stdout,q);
                else{
                    printf("QUOTAZIONE del ");
                    DAY_print(stdout,d1);
                    printf(" non presente in BST\n");
                }
            }
                break;
            case 3:{
                printf("Inserire data valida: ");
                d1 = DAY_init(stdin);
                printf("Inserire data valida: ");
                d2 = DAY_init(stdin);
                if(DAY_cmp(d1,d2)>0){
                    DATA tmp = d1;
                    d1 = d2;
                    d2 = tmp;
                }
                if( DAY_cmp(QUOTAZIONI_minD(stock->quotazioniBST),d2)<=0 &&
                    DAY_cmp(QUOTAZIONI_maxD(stock->quotazioniBST),d1)>=0){
                    QUOTAZIONI_bestQ(&max,&min,d1,d2,stock->quotazioniBST);
                    printf("maxQ: %0.3f \t minQ: %0.3f\n",max,min);
                }
                else printf("NO MATCH\n");
            }
                break;
            case 4:TITOLO_storage(stdout,stock,e_titolo);
                break;/*Mantengo sempre aggiornati max e min maxDay e minDay*/
            case 5: QUOTAZIONI_balance(stock->quotazioniBST);
                break;
        }
    } while (fineUI != 1);
    QUOTAZIONI_balance(stock->quotazioniBST);
}