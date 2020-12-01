#include <stdio.h>
#include <stdlib.h>

int gcd(int a,int b);
void scambia(int *a1, int *a2);
int even(int a);

int main()
{
    int a,b,input=0,MCD;

    do{
        printf("Inserire due interi positivi: ");
        if(scanf("%d %d",&a,&b)==2 && a>0 && b>0)
            input=1;
        else printf("Errore: input non corretto.\n");
    }while(!input);

       MCD = gcd(a,b);
       if(MCD>1)
        printf("MASSIMO COMUN DIVISORE: %d",MCD);
       else
        printf("NUMERI PRIMI TRA LORO");

    return 0;
}

int gcd(int a,int b){

    if(a<b)scambia(&a,&b);

    if((a%b)==0)
        return b;

    if(even(a)){
        if(even(b)){
            /*Entrambi Pari*/
            a = a/2;
            b = b/2;
            return 2*(gcd(a,b));
        }
        /*a Pari b Dispari*/
        scambia(&a,&b);
        b = b/2;
        return gcd(a,b);
    }else if(even(b)){
        /*b Pari a Dispari*/
        b = b/2;
        return gcd(a,b);
    }else {
        /*a Dispari b Dispari*/
       a = (a-b)/2;
       return gcd(a,b);
    }



}

void scambia(int *a1, int *a2){
    int Item = *a1;
    *a1 = *a2;
    *a2 = Item;
return;
}

int even(int a){
return 1-(a%2);
}
