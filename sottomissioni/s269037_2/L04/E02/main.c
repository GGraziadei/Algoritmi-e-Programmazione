#include <stdio.h>
#include <stdlib.h>

#define N 7

int majority(int *a,int n);
int contaOcc (int *a,int n,int Item);

int main()
{
    //int vet[N] = {0,0,0,0,0,4,1,1};
    int vet[N] = {3,0,0,1,0,5,3}; //Modificando il vettore test effettuare modifica dimensione alla riga 4
    int res = majority(vet,N);
    printf("Elemento maggioritario: %d%s",res,(res!=-1)?".":" (non esiste un elemento maggioritario).");

    return 0;
}


int majority(int *a, int n){
    int maxOccDx = 0, maxOccSx=0, CmaxOccDx = 0,
        CmaxOccSx=0, inf = 0, sup = n-1, m;

    /*Condizione di terminazione */
    if(inf == sup)
        return a[inf];

    m = (sup+inf)/2;

    /*divide and conquer a=2 b=2
    Ricerco nei sottovettori l'elemento con più occorrenze*/
    maxOccSx = majority(a,m+1);
    maxOccDx = majority((a+m+1),sup-m);

    if(maxOccDx == maxOccSx)
        return maxOccDx;

    /*Verifica alla radice*/
    if(n==N){
        CmaxOccSx = contaOcc(a,n,maxOccSx);
        CmaxOccDx =contaOcc(a,n,maxOccDx);
        if(CmaxOccDx>n/2)
            return maxOccDx;
        else if(CmaxOccDx>n/2)
            return maxOccSx;
        else
            return -1;
    }

    CmaxOccSx = contaOcc(a,m,maxOccSx);
    CmaxOccDx = contaOcc((a+m+1),m,maxOccDx);

    if(CmaxOccDx>CmaxOccSx )
        return maxOccDx;
    else
        return maxOccSx;

}

int contaOcc (int *a,int n,int Item){
    int i,conta=0;

    for(i=0; i<n; i++)
        if(a[i]==Item)
            conta = conta+1;

    return conta;
}
