//LAB 0 ES 2 GIANLUCA GRAZIADEI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N_MAX 20+1

int conta(char* , int );
int main()
{
   FILE* fp = fopen("testo.txt","r");

   int n,Conta,i,j,N;
   fscanf(fp,"%d",&N);
   printf("Ho trovato %d parole ! Per favore inserisci il numero n: ",N);
   scanf("%d",&n);

   Conta = 0;
   for(i=0;i<N;i++){
    char tmp[N_MAX];
    fscanf(fp,"%s",tmp);
    printf("\n");
     printf("--------------- %s ---------------", tmp);
     printf("\n");
    Conta += conta(tmp, n);
      printf("\n");
        printf("------------------------------");
          printf("\n");
   }
    printf("Sono presenti %d sottostringhe di lunghezza %d con almeno 2 vocali",Conta,n);
   fclose(fp);

    return 0;
}

int conta(char* stringa, int n){
int c = 0;

    int l = strlen(stringa); //lunghezza stringa
    int max;
    if (l<n)max=l;
    else max=n;

    int k,vocale,j,i,finito =0;
    vocale =0;

    for(i=0; i<l; i++){
        //analizzo tutte le possibili sottostringhe
            for(j=0; j<n && !finito; j++){
                    //Aggiungo ad i il valore di j in modo tale da spostatmi nella stringa
                    if(stringa[i+j]==NULL) finito=1;
                    else{
                        char test = tolower(stringa[i+j]);
                        if( test == 'a' || test =='e' || test == 'i' || test =='o' || test =='u' ){
                            vocale++;
                        }
                    }

                if(vocale == 2 ) {
                    finito = 1;

                    for (k=0; k<n && stringa[i+k]!= NULL;k++) printf("%c",stringa[i+k]);
                    if(k >=n) c++;
                    else printf("-- -- NON VALIDA! La sottostringa non raggiunge i %d elementi",n);
                }
            }

        printf("\n");


        vocale = 0;
        finito = 0;

    }

return c;
}
