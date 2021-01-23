#include <stdio.h>
#include "GRAPH.h"
#define S 30+1
#define N_SCELTE 4
#define DBG 0
#define GRAFO "grafo.txt"
#define GRAFO_STORAGE "GRAFO_export.txt"
int creaTab(ST st_tab);
void leggiGrafo(GRAPH G);

void stampaMenu(char *scelte[], int *selezione){
    int i=0;
    printf("\nMENU'\n");
    for(i=0;i<N_SCELTE;i++)
        printf("%2d > %s\n",i,scelte[i]);
    printf("ID_operazione: ");
    scanf(" %d",selezione);
}
int main() {
    setbuf(stdout,0);
    int selezione;
    char buf1[S],buf2[S],buf3[S];
    bool fineProgramma;
    ST st_tab = ST_init(1);
    GRAPH G = GRAPH_init(creaTab(st_tab),st_tab);
    leggiGrafo(G);
    #if DBG
    GRAPH_store(stdout,G);
    #endif
    char *scelte[] = {
            "Uscita",
            "Stampa in ordine alfabetico dei vertici e degli archi che su di esso insistono",
            "Crea LISTA_ADJ",
            "Verifica adiacenza a coppie"
    };
    fineProgramma = falso;
    source_info source;
    do {
        stampaMenu(scelte, &selezione);
        switch (selezione) {
            case 0: {
                fineProgramma = vero;
            }break;
            case 1:{
                printf("Da quale SD preferisci acquisire i dati?\nMADJ(0)%s: ",GRAPH_isLadj(G)==vero?" LADJ(1) ":" ");
                scanf("%d",&source);
                GRAPH_info(stdout,G,source);
            }break;
            case 2: GRAPH_lAdj(G);
                break;
            case 3: {
                printf("\nInserisci i nomi dei 3 vertici (separati da spazio): ");
                assert(scanf("%s %s %s",buf1,buf2,buf3) == 3);
                printf("Da quale SD preferisci acquisire i dati?\nMADJ(0)%s: ",GRAPH_isLadj(G)==vero?" LADJ(1) ":" ");
                scanf("%d",&source);
                printf("I vertici %s adiacenti a coppie",(GRAPH_isAdj(G,buf1,buf2,source)&&GRAPH_isAdj(G,buf1,buf3,source)
                        &&GRAPH_isAdj(G,buf3,buf2,source))==vero?"SONO":"NON SONO");
            }break;
        }

    }while(!fineProgramma);
    GRAPH_free(G); /*La gestione di ST passa al grafo per cui è suo compito la distruzione*/
    return 0;
}

int creaTab(ST st_tab){
    char elab1[S],elab2[S],rete1[S],rete2[S];
    FILE *fp = fopen(GRAFO,"r");
    assert(fp!=NULL);
    while(fscanf(fp,"%s %s %s %s %*d",elab1,rete1,elab2,rete2) == 4){
        if(ST_search(st_tab,elab1) == -1)
            ST_insert(st_tab,elab1,rete1);
        if(ST_search(st_tab,elab2) == -1)
            ST_insert(st_tab,elab2,rete2);
    }
    fclose(fp);
    return ST_count(st_tab);
}

void leggiGrafo(GRAPH G){
    FILE *fp = fopen(GRAFO,"r");
    assert(fp!=NULL);
    GRAPH_read(fp,G);
    fclose(fp);
}