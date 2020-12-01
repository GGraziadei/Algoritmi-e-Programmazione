#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define S 30+1
typedef enum {
    bianco,nero
}e_colore;
typedef struct {
    int **matr,*bianchi,*neri;
    int nr;
    int nc;
} Tab;

int **malloc2dR (int nr,int nc);
int isEven(int a);
int dimensioniVet (int nr,int nc,e_colore colore);
void  malloc2dP (int ***m,int nr,int nc);
void  free2d(int **m,int nr,int nc);
void separa(int **m,int nr,int nc,int **b,int**n);
void stampaVet (Tab *tab,e_colore colore);

int main() {
    Tab tab;
    char str[S];
    FILE *fp;
    int i,j;
    do{
        printf("Inserire percorso file valido: ");
        gets(str);
        fp = fopen(str,"r");
        if(fp == NULL)printf("Percorso file non valido\n\n");
    }while (fp == NULL);
    if(fscanf(fp,"%d %d",&tab.nr,&tab.nc) != 2)
        exit(-1);
    malloc2dP(&(tab.matr),tab.nr,tab.nc);
    free2d(tab.matr,tab.nr,tab.nc);
    tab.matr = malloc2dR(tab.nr,tab.nc);
    for(i=0;i<tab.nr;i++)
        for (j = 0; j < tab.nc; j++)
            if(fscanf(fp,"%d",&tab.matr[i][j]) !=1)
                exit(-1);
    fclose(fp);
    separa(tab.matr,tab.nr,tab.nc,&tab.bianchi,&tab.neri);
    printf("Bianchi: ");
    stampaVet (&tab,bianco);
    free(tab.bianchi);
    printf("\nNeri: ");
    stampaVet(&tab,nero);
    free(tab.neri);
    free2d(tab.matr,tab.nr,tab.nc);

    return 0;
}


void  malloc2dP (int ***m,int nr,int nc){
    int i;
    *m = (int **)malloc(nr*sizeof(int*));
    for(i=0;i<nr;i++)
        (*m)[i] = (int *) malloc(nc*sizeof(int));
}

int **malloc2dR (int nr,int nc){
    int **m,i;
    /*Vettore di puntatori*/
    m = (int **)malloc(nr*sizeof(int*));
    for(i=0;i<nr;i++)
        m[i] = (int *) malloc(nc*sizeof(int));
    return m;
}

void  free2d(int **m,int nr,int nc){
    int i;
    for(i=0;i<nr;i++)
        free(m[i]);
    free(m);
}

void separa(int **m,int nr,int nc,int *(*b),int*(*n)){
    /* i + j PARI => BIANCO
     * i + j DISPARI => NERO la dimensione dei vettori è nota ed è in fuznione di nr e nc.
     * Il primo elemento in alto a sx è una casella bianca
     *B N B
     *N B N
     *B N B*/
    int i,j,kb=0,kn=0;
    /*Scrivi l'indirizzo della regione allocata nella casella a cui puntano i puntatori*/
    *(b) = (int *) malloc(dimensioniVet(nr,nc,bianco)*sizeof(int));
    if (*(b)== NULL)
        exit(-1);
    *(n) = (int *) malloc(dimensioniVet(nr,nc,nero)*sizeof(int));
    if (*(n)== NULL)
        exit(-1);
    printf("GRAFICO\n");
    for(i=0;i<nr;i++){
        for(j=0;j<nc;j++){
            if(isEven(i+j)){
                printf("B%d ",kb);
                (*b)[kb++] = m[i][j];
            }
            else {
                printf("N%d ",kn);
                (*n)[kn++] = m[i][j];
            }
        }
        printf("\n");
    }
}

int isEven(int a){
    return 1-a%2;
}

void stampaVet (Tab *tab,e_colore colore){
    int i,k,*vet;
    k = dimensioniVet(tab->nr,tab->nc,colore);
    switch (colore) {
        case bianco: vet = tab->bianchi;
            break;
        case nero: vet = tab->neri;
            break;
    }
    for(i=0;i<k;i++)
        printf("%d ",vet[i]);

    return;
}

int dimensioniVet (int nr,int nc,e_colore colore){
    int nb,nn;
    nn = (nr*nc)/2;
    nb = (nr*nc) - nn;
    switch (colore) {
        case bianco:return nb;
        case nero:return nn;
    }
}