#include <stdio.h>
#include "titoliLIST.h"
#define N_SCELTE 3
#define S 20+1
#define DBG 0
int stampaMenu(char **scelte);
int main() {
    setbuf(stdout,0);
    TITOLI stockList = TITOLI_init();
    int fineProgramma = 0,numAdd,i;
    char buf[S];
    FILE *fin;
    char *scelte[] = {
            "Uscita",
            "Seleziona un TITOLO ed esegui OPERAZIONI di gestione",
            "Aggiungi TITOLI/TRANSAZIONI"
    };
    do {
        switch (stampaMenu(scelte)) {
            case 0:
                fineProgramma = 1;
                break;
            case 1: {
                printf("Inserire il codice titolo: ");
                scanf("%s", buf);
                TITOLI_UI(stockList, buf);
            }
                break;
            case 2: {
                printf("Inserire source: ");
                scanf("%s", buf);
                fin = fopen(buf, "r");
                assert(fin != NULL);
                fscanf(fin,"%d",&numAdd);
                for(i=0; i<numAdd; i++)
                    TITOLI_add(fin, stockList);
                fclose(fin);
                TITOLI_print(stdout,stockList);
            }
                break;
        }
    } while (fineProgramma == 0);
    TITOLI_free(stockList);
    return 0;
}

int stampaMenu(char **scelte){
    int i = 0;
    for(i=0; i<N_SCELTE; i++)
        printf("%d > %s\n",i,scelte[i]);
    do {
        scanf("%d", &i);
    }while(i<0 && i>N_SCELTE);
    return i;
}
