#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50
#define MAXR 50
#define S 100+1


typedef struct {
int r;
int c;
int base;
int altezza;
}Regione;

void leggiMatrice (int M[][MAXR], int *nr,int *nc);
void stampaRes (Regione);
int cercaRegioni(Regione*,int mappa[][N],int,int);
int riconosciRegione (int M[][MAXR],int nr, int nc,int r,int c,int*b,int*h);
void trovaMax(Regione r[], int *A,int *H,int *L,int n);
int main()
{
    int maxH,maxA,maxL,nRegioni,nr,nc,M[MAXR][MAXR];
    Regione regioni[N];
    maxH = maxA = maxL = 0;

    leggiMatrice (M,&nr,&nc);
    /*Identifico in stampa una regione con un vettore diagonale
    applicato nel vertice il alto a sx il quale punta al vertice in basso a destra della regione
    I vettori sono epsressi in un SR che ha l'asse delle y verso il basso*/
    nRegioni = cercaRegioni(regioni,M,nr,nc);
    trovaMax (regioni,&maxA,&maxH,&maxL,nRegioni);
    printf("\nMax Base: ");    stampaRes(regioni[maxL]);
    printf("Max Area: ");    stampaRes(regioni[maxA]);
    printf("Max Altezza: "); stampaRes(regioni[maxH]);

    return 0;
}

void leggiMatrice (int M[][MAXR], int *nr,int *nc){
    char str[S];
    FILE *fp;
    int i,j,*riga;

        do{
            printf("Inserire percorso file: ");
            scanf("%s",str);
            fp = fopen(str,"r");
            if(fp == NULL) printf("File non trovato\n");

        }while(fp == NULL);

        if( fscanf(fp,"%d %d",nr,nc) == EOF ) exit(-1);

        for(i=0;i<*nr; i++){
         riga = M[i];
         /* puntatore alla riga i-esima
         equivale a &(M[i][0])*/
            for(j=0;j<*nc;j++){
                if( fscanf(fp,"%d ",(riga+j)) == EOF)
                    exit(-1);
                if(*(riga+j)!= 0 && *(riga+j)!= 1){
                    printf("Verificare file riga %d colonna %d",i+1,j+1);
                    exit(-1);
                }
            }
        }


    fclose(fp);

return;
}

int cercaRegioni(Regione r[],int mappa[][MAXR],int nr,int nc){
    int ii,jj,i,j,ll,hh,n =0; //n indice del vettore Regione r[]

    printf("Riconosci Regione: \n");
    for(i=0; i<nr; i++){
        for(j=0; j<nc; j++){
            if(riconosciRegione(mappa,nr,nc,i,j,&ll,&hh)){
                //Salvo dimensioni
                r[n].r = i;
                r[n].c = j;
                r[n].base = ll;
                r[n].altezza = hh;
                /*Deseleziono la regione individuata.*/
                for(ii=i; ii<r[n].altezza+r[n].r; ii++)
                   for(jj=j; jj<r[n].base+r[n].c; jj++)
                    mappa[ii][jj] = 0;
                printf("Regione %d @ (%d,%d)-->(%d,%d)\n",n,r[n].r,r[n].c,r[n].altezza+r[n].r-1,r[n].base+r[n].c-1);
                n = n+1;

            }
        }
    }

return n;
}


void stampaRes (Regione r){

    printf("estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n\n",
           r.r,r.c,r.base,r.altezza,r.base*r.altezza);

}

int riconosciRegione (int M[][MAXR],int nr, int nc,int r,int c,int*b,int*h){
    int ii,jj;

    if(M[r][c]== 1){
                for(ii=r; ii<nr && M[ii][c]==1; ii++);
                for(jj=c; jj<nc && M[r][jj]==1; jj++);
                *b = jj-c;
                *h = ii-r;
       return 1;
    }

    return 0;
}

void trovaMax(Regione r[], int *A,int *H,int *L,int n){
    int i,maxA,maxL,maxH;
    maxA = maxH = maxL = 0;
    for(i=0; i<n; i++){
        if(r[i].altezza>maxH){
            maxH = r[i].altezza;
            *H = i;
        }
        if(r[i].base>maxL){
            maxL = r[i].base;
            *L = i;
        }
        if(r[i].altezza*r[i].base>maxA){
            maxA = r[i].altezza*r[i].base;
            *A = i;
        }
    }
return;
}

