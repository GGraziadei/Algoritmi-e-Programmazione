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
int cercaRegioni(Regione*,int mappa[][N],int,int,int*,int*,int*);
int riconosciRegione (int M[][MAXR],int nr, int nc,int r,int c,int*b,int*h);

int main()
{
    int maxH,maxA,maxL,nRegioni,nr,nc,M[MAXR][MAXR];
    Regione regioni[N];

    leggiMatrice (M,&nr,&nc);
    /*Identifico in stampa una regione con un vettore diagonale
    applicato nel vertice il alto a sx il quale punta al vertice in basso a destra della regione
    I vettori sono epsressi in un SR che ha l'asse delle y verso il basso*/
    nRegioni = cercaRegioni(regioni,M,nr,nc,&maxA,&maxH,&maxL);
    printf("------- Numero regioni acquisite: %d -------\n\n",nRegioni);
    printf("Max Base: ");    stampaRes(regioni[maxL]);
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

        fscanf(fp,"%d %d",nr,nc);
        riga = (int*)*(M[0]);
        for(i=0;i<*nr; i++){
         riga = (int *)&(M[i]);
         /* puntatore al primo elemento di riga i-esima */
            for(j=0;j<*nc;j++){
                fscanf(fp,"%d ",(riga+j));
                printf("%d  ", *(riga+j));
                if(*(riga+j)!= 0 && *(riga+j)!= 1){
                    printf("Verificare file riga %d colonna %d",i+1,j+1);
                    exit(-1);
                }
            }
            printf("\n");
        }


    fclose(fp);

return;
}

int cercaRegioni(Regione r[],int mappa[][MAXR],int nr,int nc, int *A,int *H,int *L){
    int ii,jj,i,j,a,h,l,ll,hh,aa,n =0; //n indice del vettore Regione r[]
    a = l = h = *A = *H = *L =0;

    for(i=0; i<nr; i++){
        for(j=0; j<nc; j++){
            if(riconosciRegione(mappa,nr,nc,i,j,&ll,&hh)){
                //Salvo dimensioni
                r[n].r = i;
                r[n].c = j;
                r[n].base = ll;
                r[n].altezza = hh;
                aa = ll*hh;

                for(ii=i; ii<r[n].altezza+r[n].r; ii++)
                   for(jj=j; jj<r[n].base+r[n].c; jj++)
                    mappa[ii][jj] = 0;


                //Trovo max_X index

                if(ll>l){
                    l = ll;
                    *L = n;
                }
                if(hh>h){
                    h = hh;
                    *H = n;
                }

                if(aa>a){
                    a = aa;
                    *A = n;
                }

                printf("Regione %d @ (%d,%d)-->(%d,%d)\n",n,r[n].r,r[n].c,r[n].altezza+r[n].r-1,r[n].base+r[n].c-1);
                stampaRes(r[n]);
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
                /*Definisco regione qualsiasi rettangolo anche quadrato
                di dimensione 1x1 */
                for(ii=r; ii<nr && M[ii][c]==1; ii++);
                for(jj=c; jj<nc && M[r][jj]==1; jj++);
                *b = jj-c;
                *h = ii-r;
       return 1;
    }

    return 0;
}
