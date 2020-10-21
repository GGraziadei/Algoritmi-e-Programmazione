#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD 10 //max numero di occorrenze per seq
#define N 20
#define SC 5+1
#define SS 200+1
#define SP 25+1
#define TESTO "testo.txt"
#define SEQUENZE "sequenze.txt"
#define SHOWINDEX 0 //stampa posizione assegnata alle parole lette 1=VERO 0=FALSO

typedef struct{
int l;
int occNow;
int occIndex [MAX_WORD];
char words[MAX_WORD][SP];
char seq [SC];
}Sequenza;

void trovaOccorrenze (Sequenza *,int );
int leggiSeq ( Sequenza *);
void stampaRes (Sequenza* , int );
void toMinuscolo (char *);

int main()
{
    Sequenza sequenze[N];
    int n = leggiSeq(sequenze);
    trovaOccorrenze(sequenze,n);
    stampaRes (sequenze,n);

    return 0;
}



int leggiSeq( Sequenza p[]){
    int j,i,n =0;
    FILE *fin = fopen(SEQUENZE,"r");
    if(fin==NULL)EXIT_FAILURE;

    fscanf(fin,"%d",&n);

    for(i=0;i<n;i++){
        if(fscanf(fin,"%s",p[i].seq)!=1)
           exit(-1);
        toMinuscolo(p[i].seq);
        p[i].l = strlen(p[i].seq);
        p[i].occNow = 0;
        for(j=0; j<MAX_WORD; j++)p[i].occIndex[j] =-1;
    }
    if(SHOWINDEX)printf("sequenze lette: %d\n",n);

    fclose(fin);
    return n;
}

void trovaOccorrenze (Sequenza p[],int n){
    int parole,l,ll,i,ii,j,jj,m;
    char word[SP];
    char wordCpy[SP];
    char str[SS];
    char *strTest;
    FILE *fin = fopen(TESTO,"r");
    if(fin==NULL)EXIT_FAILURE;
    parole =0;

    /* Leggo riga per riga per estrapolare correttamente le words */

        while(fgets(str,SS,fin)!= NULL){
            l = strlen(str);
            j=0;

            for(i=0; i<l; i++){ /* Costruzione words riga*/

                if( isspace(str[i])  || ispunct(str[i]) ){

                    /*
                    i è l'indice del carattere di terminazione
                    j  conteggia i caratteri letti e contestualmente rilasciati in word
                    Salvo in ii il numero dei caratteri selezionati
                    I caratteri non standard ASCII sicuramente non costituiscono parte di un parola
                    essendo questa per definizione composta da alnum contigui, una volta riconosciuti
                    ricodifico con uno spazio e mando il ciclo indietro di 1.
                    */

                    for(ii=j,jj=0; jj<ii && str[i-j]!=' '; jj++,j--)
                        word[jj]=str[i-j];
                    word[jj] = '\0';/* jj = strlen(word) per controllo jj = ii +1*/

                     if(!j && strlen(word)>0){
                        strcpy(wordCpy,word);/* toMinuscolo fa perdere le informazioni sulle maiuscole*/
                        toMinuscolo(word);
                        parole = parole+1;
                        if(SHOWINDEX)printf("%s (pos: %d)\n",word,parole);

                         for(ii=0; ii<jj; ii++){
                            strTest = &(word[ii]);
                            ll = strlen(strTest);

                                for(m=0; m<n; m++){
                                    if(p[m].l<=ll && p[m].occNow <MAX_WORD && p[m].occIndex[p[m].occNow-1] != parole){
                                    /* Verifico compatibilità alla strncmp
                                    con (p[m].occIndex[p[m].occNow-1] != parole) verifico che l'occorrenza non sia già stata
                                    processata per la word. */
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
                else if (isalnum(str[i])) j=j+1;
                else { //Altrimenti carattere non standard ASCII lo ricodifico con uno spazio
                  str[i]=' ';
                  i = i-1;
                }

            }
        }

    fclose(fin);
return;
}

void stampaRes (Sequenza p[], int n){
    int i,jj;

    for(i=0; i<n; i++){//sequenza
    /* If condizionale <condizione> ? <Se_Vero>:<Se_falso>*/
    printf("Sequenza %s",p[i].seq);
    printf("%s",p[i].occNow>=1?" contenuta in: ":" non presenta occorrenze.");

        for(jj=0; jj<p[i].occNow && p[i].occIndex[jj]>0; jj++){//word_sequenza
            printf("%s (Parola in posizione %d)%c ",p[i].words[jj],p[i].occIndex[jj],
                   jj==p[i].occNow-1?'.':',');
        }
    printf("\n\n");
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
