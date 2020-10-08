//Gianluca Graziadei

#include <stdio.h>
#include <stdlib.h>
#define N 30

void creaSoluzione (int* , int* , int l);
int leggiVettore (int*);
int trovaMax (int*, int l);
void stampaSoluzione (int* , int* , int l, int max);

int main()
{
    int vettore[N] = {0};
    int l = leggiVettore (vettore);
    int soluzione[N+1] = {0}; // N+1 => fix per vettori lunghi N
    creaSoluzione(vettore,soluzione,l);
    int max = trovaMax (soluzione,l);
    printf("\n I sottovettori sono lunghi %d\n",max);
    stampaSoluzione(vettore,soluzione,l,max);
    return 0;

}


int leggiVettore (int vettore[]){
int j,finito,i=0;
finito = 0;

    while (i<N && !finito) {
            printf("Add %d elemento del vettore:  ",i+1);
        int tmp=0;
        scanf("%d",&tmp);
        if(tmp<0)finito=1;
        else{
            vettore[i]=tmp;
            i++;
        }
    }

for(j=0;j<i;j++)printf("%d \n",vettore[j]);

    return i; //i lunghezza effettiva del vettore
}

void creaSoluzione (int vettore[], int soluzione[], int l){
int i,j,k;
j=0;
    for(i=0;i<l;i++){
        if(!vettore[i]) //Se trovo elemto 0 aggiungo su contatore indice
        {
                soluzione[i] = j; //inserisco nella casella relativa all'indice il numero di passi precedenti
                j=0;
        }
        else j++;
    }
    soluzione[i] = j; //fix per non trascurare ultima sequenza se vettore non termina con 0 => il vettore soluzione ha un terminatore in più del vettore

}

int trovaMax (int vettore[], int l){
int i,max;
max =0;

    for(i=0;i<l+1;i++){
        if(vettore[i]>max)max=vettore[i];
    }

return max;
}

void stampaSoluzione (int vettore[], int soluzione[], int l, int max){
int i,j;

    for(i=0;i<l+1;i++){
        if(soluzione[i]== max){ //Se l'indice è soluzione, quindi diverso da 0
            for(j=i-max; !soluzione[j]; j++) {
                printf(" -> %d indice:%d ", vettore[j],j);
            }
            printf("\n\n");
        }
    }

}


