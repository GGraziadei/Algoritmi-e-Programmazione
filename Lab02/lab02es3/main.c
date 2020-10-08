#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD 10 //max numero di occorrenze
#define N 20
#define SC 5
#define SS 200
#define SP 25
#define TESTO "testo.txt"
#define SEQUENZE "sequenze.txt"
#define SPAZIO " "

typedef struct{
int l;
int occIndex [MAX_WORD];
char words[MAX_WORD][SP];
int occNow;
char seq [SC];
}Parola;

void trovaOccorrenze (Parola *,int );
int leggiParole( Parola *);
void stampaRes (Parola* , int );
void toMinuscolo (char *);

int main()
{
    Parola parole[N];
    int n = leggiParole(parole);
    trovaOccorrenze(parole,n);
    stampaRes (parole,n);

    return 0;
}



int leggiParole( Parola p[]){
    int j,i,n =0;
    FILE *fin = fopen(SEQUENZE,"r");
    if(fin==NULL)EXIT_FAILURE;

    fscanf(fin,"%d",&n);

    for(i=0;i<n;i++){
        if(fscanf(fin,"%s",p[i].seq)!=1)
           exit("Check sequenze.txt necessario");
        toMinuscolo(p[i].seq);
        p[i].l = strlen(p[i].seq);
        p[i].occNow = 0;
        for(j=0; j<MAX_WORD; j++)p[i].occIndex[j] =-1;
    }
    printf("sequenze lette: %d\n",n);

    fclose(fin);
    return n;
}

void trovaOccorrenze (Parola p[],int n){
    int parole,l,ll,i,ii,j,jj,m;
    char word[SP];
    char wordCpy[SP];
    char str[SS];
    char *strTest;
    FILE *fin = fopen(TESTO,"r");
    if(fin==NULL)EXIT_FAILURE;
    parole =0;

    /* Non avendo vincoli in out leggo parola per parola */

        while(fgets(str,SS,fin)!= NULL){
            l = strlen(str);
            j=0;

            for(i=0; i<l; i++){ /* Costruzione words riga*/
               if(!isprint(str[i])) str[i]=' ';

                if( isspace(str[i])  || (ispunct(str[i])&& str[i]!=' ') ){

                    /* j n+1 degli alnum prima della terminazione i �
                    l'indice del carattere da saltare
                    j "si muove come una molla", carica i caratteri e rilascia words */
                    ii = j;

                    for(jj=0; jj<ii && str[i-j]!=' '; jj++){
                            word[jj]=str[i-j];
                            j = j-1;
                    }
                    word[jj] = '\0';/* jj = strlen(word) per controllo jj = j+1*/

                     if(!j && strlen(word)>0){ //elimino dal conteggio le "fake words"
                        strcpy(wordCpy,word);
                        toMinuscolo(word);
                        parole = parole+1;
                        printf("%s n: %d\n",word,parole);

                         for(ii=0; ii<jj; ii++){
                         //rif. esercizio 2 lab1 + pag. 50 dal problema al programma
                            strTest = &(word[ii]);
                            ll = strlen(strTest);

                                for(m=0; m<n; m++){
                                    if(p[m].l<=ll && p[m].occNow <MAX_WORD && p[m].occIndex[p[m].occNow-1] != parole){
                                    /* Verifico compatibilit� alla strncmp
                                    con p[m].occIndex[p[m].occNow-1] != parole verifico che l'occorrenza non sia stata gi� segnata per
                                    la word corrente. */
                                      if(!strncmp(strTest,p[m].seq,p[m].l)){
                                        strcpy(p[m].words[p[m].occNow],wordCpy);
                                        p[m].occIndex[p[m].occNow]=parole;
                                        p[m].occNow = p[m].occNow +1;
                                      }
                                    }
                                }
                            }
                      }
                }
                else if (isalnum(str[i])) j=j+1; //continua a "caricare" caratteri
            }
        }

    fclose(fin);
return;
}

void stampaRes (Parola p[], int n){
    int i,jj;

    for(i=0; i<n; i++){//sequenza
    if(p[i].occIndex[jj]>0)printf("La sequenza ' %s ' contenuta in: \n",p[i].seq);
    else printf("La sequenza ' %s ' non contiene occorrenze\n",p[i].seq);
        for(jj=0; jj<p[i].occNow && p[i].occIndex[jj]>0; jj++){
            printf("%s (parola in posizione %d)\n",p[i].words[jj],p[i].occIndex[jj]);
        }
    printf("_____________________________\n\n");
    }

}

void toMinuscolo (char str[]){
    int n,i;
    n = strlen(str);
    for(i=0; i<n; i++)
        if(str[i]<'a')
         str[i] = tolower(str[i]);
 return;
}
