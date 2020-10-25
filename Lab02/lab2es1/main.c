#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50
#define S 100+1

typedef struct {
/* il vertice in alto a sx ha coordinate i[0],j[0]
il vertice in basso a dx ha coordinate i[1]-1,j[1]-1
altezzaRegione i[1]-i[0]
larghezzaRegione j[1]-j[0]
areaRegione (i[1]-i[0]) * (j[1]-j[0])
Tutte le informazioni sono espresse in funzione degli indici
Le regioni sono al più N
*/
int i[2];
int j[2];
}Regione;

void creaMappa(int mappa[][N], int*);
void stampaRes (Regione);
int cercaRegioni(Regione*,int mappa[][N],int,int,int*,int*,int*);

int main()
{
    int maxH,maxA,maxL,nRegioni, dim[2], mappa[N][N] = {-2};
    Regione regioni[N];

    creaMappa(mappa,dim); //dim[0] = nr, dim[1] = nc
    nRegioni = cercaRegioni(regioni,mappa,dim[0],dim[1],&maxA,&maxH,&maxL);

    printf("Max Base: ");    stampaRes(regioni[maxL]);
    printf("Max Area: ");    stampaRes(regioni[maxA]);
    printf("Max Altezza: "); stampaRes(regioni[maxH]);

    return 0;
}

void creaMappa(int mappa[][N], int dim[]){
    char str[S];
    FILE *fp;
    int i,j; //dim[0] = nr, dim[1] = nc

        do{
            printf("Inserire percorso file: ");
            scanf("%s",str);
            fp = fopen(str,"r");
            if(fp == NULL) printf("File non trovato\n");

        }while(fp == NULL);

        fscanf(fp,"%d %d",&dim[0],&dim[1]);

        for(i=0;i<dim[0]; i++){
            for(j=0;j<dim[1];j++){
                fscanf(fp,"%d ",&mappa[i][j]);
                //printf("%d  ", mappa[i][j]);
                if(mappa[i][j]!= 0 && mappa[i][j]!= 1){
                    printf("Verificare file riga %d colonna %d",i+1,j+1);
                    exit(-1);
                }
            }
            //printf("\n");
        }


    fclose(fp);

return;
}

int cercaRegioni(Regione r[],int mappa[][N],int nr,int nc, int *A,int *H,int *L){
    int ii,jj,i,j,a,h,l,ll,hh,aa,n =0; //n indice del vettore Regione r[]
    a = l = h = *A = *H = *L =0;

    for(i=0; i<nr; i++){
        for(j=0; j<nc; j++){

            if(mappa[i][j]== 1){
                //Trovo dimensioni
                for(ii=i; i<nr && mappa[ii][j]==1; ii++);
                for(jj=j; j<nc && mappa[i][jj]==1; jj++);

                //Salvo dimensioni
                r[n].i[0]=i;
                r[n].j[0]=j;
                r[n].i[1]=ii;
                r[n].j[1]=jj;

                for(ii=i; ii<r[n].i[1]; ii++)
                   for(jj=j; jj<r[n].j[1]; jj++)
                    mappa[ii][jj] = 0;

                ll = r[n].j[1]-r[n].j[0];
                hh = r[n].i[1]-r[n].i[0];
                aa = ll*hh;

                //Trovo max_X

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

                //printf("Regione %d @ (%d,%d)-> (%d,%d)\n",n,r[n].i[0],r[n].j[0],r[n].i[1]-1,r[n].j[1]-1);
                //stampaRes(r[n]);
                n = n+1;

            }

        }
    }
printf("------- Numero regioni acquisite: %d -------\n\n",n);
return n;
}


void stampaRes (Regione r){

    printf("estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n\n",
           r.i[0],r.j[0],r.j[1]-r.j[0],r.i[1]-r.i[0],(r.i[1]-r.i[0])*(r.j[1]-r.j[0]));

}
