#include <stdio.h>
#include <stdlib.h>

# define maxKey 100
#define S 30+1
#define MAXN 1000

typedef int Item;

int ITEMeq (Item A , Item B);
int ITEMneq (Item A, Item B);
int ITEMlt (Item A, Item B);
int ITEMgt (Item A, Item B);
void ITEMshow (Item A[],int n);
void stampaSelectionSort(Item A[], int n);
void stampaInsertionSort(Item A[], int n);
void stampaShellSort(Item A[], int n);
void scambiaItem (Item *pa, Item *pb);



int main()
{
    FILE *fp;
    char str[S];
    int i,j,r,n;
    Item arraySelection [MAXN];
    Item arrayInsertion [MAXN];
    Item arrayShell [MAXN];

    do{
            printf("Inserire il percorso del file sort.txt: ");
            gets(str);
            fp = fopen(str,"r");
            printf("%s",fp==NULL?"File non trovato\n":"File Aperto correttamente\n");

    }while(fp==NULL);

    fscanf(fp,"%d",&r); /* Numero di righe del file, ogni riga contiene una sequenza */

    for(i=0; i<r; i++){
      fscanf(fp,"%d",&n);
      /* Salvo la sequenza della riga i-esima in Item
      copio il vettore per utilizzare i 3 algoritmi di ordinamento */
      for(j=0;j<n;j++){
            fscanf(fp,"%d",&arraySelection[j]);
            arrayInsertion[j]=arraySelection[j];
            arrayShell[j]=arraySelection[j];
      }
      printf("Sequenza: ");ITEMshow(arraySelection,n);
      stampaSelectionSort(arraySelection,n);
      stampaInsertionSort(arrayInsertion,n);
      stampaShellSort(arrayShell,n);
    }

    fclose(fp);

    return 0;
}

void stampaSelectionSort(Item A[], int n){
int i,j,min,r,l,nScambi,nIterazioni;
    r=n-1;
    l=0;
    nScambi=0;
    nIterazioni=0;

printf("\n--- SELECTION SORT ---\nSequenza: ");

    for(i=l; i<=r; i++){
        nIterazioni++;
        min = i;
        /*trova il minimo nel vettore [i+1,r]
        Algoritmo stabile */
        for(j=i+1; j<=r; j++){
         nIterazioni++;
            if(ITEMlt(A[j],A[min]))
                min = j;
        }

        if(i!=min){
            scambiaItem(&A[i],&A[min]);
            nScambi = nScambi+1;
        }
        printf("Iterazioni ciclo interno %d\n",j);
    }
    printf("iterazioni ciclo esterno %d\n",i);
    printf("Scambi tot %d\n",nScambi);
    printf("Iterazioni tot %d\n",nIterazioni);
    ITEMshow(A,n);
    printf("\n\n\n");
return;
}

void stampaInsertionSort(Item A[], int n){
int i,j,x,r,l,nScambi,nIterazioni;
    r=n-1;
    l=0;
    nScambi=0;
    nIterazioni=0;

printf("\n--- INSERTION SORT ---\n");
    for(i=l+1; i<=r; i++){
        nIterazioni++;
        j = i-1;
        x = A[i];
        /*sostituisco nel vettore ordinato
        per trovare la posizione esatta di x
        nella soluzione parziale*/
        while(j>=0 && ITEMgt(A[j],x)){
            nIterazioni++;
            nScambi = nScambi+1;
            A[j+1]=A[j];
            j--;
        }
        A[j+1]=x;
        printf("Iterazioni ciclo interno %d\n",i-j);
    }
    printf("iterazioni ciclo esterno %d\n",i-1);
    printf("Scambi tot %d\n",nScambi);
    printf("Iterazioni tot %d\n",nIterazioni);
    ITEMshow(A,n);
    printf("\n\n\n");
return;
}

void stampaShellSort(Item A[], int n){
int h,i,j,x,r,l,nScambi,nIterazioni;

    r=n-1;
    l=0;
    h = 1;
    nScambi=0;
    nIterazioni=0;

printf("\n--- SHELLSORT ---\n");
    /*Utilizzo la sequenza di Knuth h=3h+1
      1. trovo il più grnade h compatibile con il vettore*/
    while(h<n/3)h=3*h+1;
    /*2. h-ordino fino h che tende a 1*/
    while(h>=1){
        printf("\n\nH= %d\n\n",h);
         for(i=l+h; i<=r; i++){
            nIterazioni++;
            j = i-h;
            x = A[i];
            /*sostituisco nel vettore ordinato
            per trovare la posizione esatta di x
            nella soluzione parziale*/
            while(j>=l && ITEMgt(A[j],x)){
                nIterazioni++;
                nScambi = nScambi+1;
                A[j+h]=A[j];
                j=j-h;
            }
            A[j+h]=x;
            printf("Iterazioni ciclo interno %d\n",i-j);
         }
        h=h/3;
     }

    printf("iterazioni ciclo esterno %d\n",i-1);
    printf("Scambi tot %d\n",nScambi);
    printf("Iterazioni tot %d\n",nIterazioni);
    ITEMshow(A,n);
    printf("\n\n\n");
return;
}

int ITEMeq (Item A, Item B) {
return (A == B);
}
int ITEMneq (Item A, Item B) {
return (A != B);
}
int ITEMlt (Item A, Item B) {
return (A <B);
}
int ITEMgt (Item A, Item B) {
return (A > B);
}
void ITEMshow (Item A[], int n){
int i;
for(i=0; i<n; i++)printf("%d ",A[i]);
return;
}
void scambiaItem (Item *pa, Item *pb){
  Item temp;
  temp = *pa;
  *pa = *pb;
  *pb = temp;
return;
}



