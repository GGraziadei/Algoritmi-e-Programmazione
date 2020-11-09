#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define S 255+1
#define BRANI "brani.txt"
#define PLAYLISTS "playlists.txt"

#define SHOW_ALBERO 0 /*Se abilitato mostra informazioni su come è formato l'insieme delle soluzioni
 * indicando gli indici delle canzoni del relativo livello selezionate.
 * In particolare mi apsetto che l'ultima playlist sia formata da: 4 2 3 4 2 */

typedef struct {
    char **canzoni;
    int num;
}Livello;

typedef struct {
    Livello *livelli;
    FILE *out;
    int N,playlists,*albero;
    char **sol;
}tabella;

tabella leggiTab();
void deallocaTab(tabella *);
void wrapper_principio_moltiplicazione (tabella *tab);
int principio_moltiplicazione (int pos,tabella *tab,int c);
void stampaPlaylist(tabella *tab, int);

int main() {
    tabella tab;
    tab = leggiTab();
    tab.out = fopen(PLAYLISTS,"w");
    printf("Le playlists possibili sono: %d. Stampo su %s",tab.playlists,PLAYLISTS);
    wrapper_principio_moltiplicazione (&tab);
    fclose(tab.out);
    deallocaTab(&tab);
    return 0;
}

tabella leggiTab(){
    int i,j;
    char c_tmp[S];
    tabella tab;
    tab.playlists =1;
    FILE *fp;
    fp = fopen(BRANI,"r");
    if (fp==NULL)
        exit(-1);
    if(fscanf(fp,"%d",&tab.N) != 1)
        exit(-1);
    tab.livelli = (Livello *)malloc(tab.N*sizeof(Livello));
    tab.sol = (char **)malloc(tab.N*sizeof(char *));
    if(SHOW_ALBERO){
        /* - - - - - ovvero vettore orizzontale di puntatori*/
        tab.albero = (int *)malloc(tab.N*sizeof(int));
    }
    for(i=0;i<tab.N;i++){
        if(fscanf(fp,"%d",&tab.livelli[i].num) != 1)
            exit(-1);
        tab.playlists = tab.playlists*tab.livelli[i].num;
        tab.livelli[i].canzoni = (char **)malloc(tab.livelli[i].num*sizeof(char*));
        for (int j = 0; j < tab.livelli[i].num; j++) {
            if(fscanf(fp,"%s",c_tmp) != 1)
                exit(-1);
            tab.livelli[i].canzoni[j] = (char *)strdup(c_tmp);
            //printf("%s \n",tab.livelli[i].canzoni[j]);
        }
    }
    fclose(fp);
    return tab;
}

void deallocaTab(tabella *tab){
    int i,j;
    /*livello i-esimo, canzione j-esima*/
    for(i=0;i<tab->N;i++){
        for (int j = 0; j < tab->livelli[i].num; j++) {
            free(tab->livelli[i].canzoni[j]);
        }
        free(tab->livelli[i].canzoni);
    }
    if(SHOW_ALBERO)
        free(tab->albero);
    free(tab->livelli);
    free(tab->sol);
    tab->N = 0;
    return;
}

void wrapper_principio_moltiplicazione (tabella *tab){
    int count = 0;
    count = principio_moltiplicazione (0,tab,count);
    if (count != tab->playlists)
        exit(-1);
    return;
}

int principio_moltiplicazione (int pos,tabella *tab,int c){
    int i;
    if(pos>=tab->N){
        stampaPlaylist(tab,c);
        return c+1;
    }
    for (int i = 0; i < tab->livelli[pos].num; ++i) {
        tab->sol[pos]=tab->livelli[pos].canzoni[i];
        if(SHOW_ALBERO)
            tab->albero[pos]=i;
        c = principio_moltiplicazione(pos+1,tab,c);
    }
    return c;
}

void stampaPlaylist(tabella *tab, int c){
    int i;
    fprintf(tab->out,"%d\t",c+1);
    if(SHOW_ALBERO)
        for ( i = 0; i < tab->N; i++)
            fprintf(tab->out,"%d ", tab->albero[i]);
    fprintf(tab->out,"\n");
    for ( i = 0; i < tab->N; i++)
        fprintf(tab->out,"\t%s\n",tab->sol[i]);
    fprintf(tab->out,"\n\n");
    return;
}