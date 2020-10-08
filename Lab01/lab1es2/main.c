#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 200+1
#define M 100+1
#define S_MAX 30


typedef struct Dizionario{
int id;
char originale[M];
int l;
}Dizionario;

void ricodifica (Dizionario d[], FILE* sorgente, FILE* out, int S);
int leggiDizionario (Dizionario d[], FILE *dizionario);

int main()
{
    FILE* sorgente = fopen("sorgente.txt","r");
    FILE* dizionario = fopen("dizionario.txt","r");
    FILE* ricodificato = fopen("ricodificato.txt","w");
    int dizionarioCheck = 1; //Dizionario presente
    int S;
    Dizionario d[S_MAX];


    // ----------------------------- verifica input
    if(sorgente == NULL){
        printf("Inserire FILE sorgente");
        return -1;
    }
    if (dizionario == NULL){
        printf("FILE dizionario mancante\n");
        dizionario = 0;
    }

    // ----------------------------- Acquisizione dizionario


    if(dizionarioCheck){
            S = leggiDizionario(d, dizionario); // S  numero di words in dizionario
            if(S > S_MAX) {
                printf("Errore formato Dizionario");
                return -1;
            }
            else if(S != -1) printf("Lettura dizionario completata correttamente \n");

            ricodifica(d,sorgente,ricodificato,S);
    }





    // -----------------------------

    fclose(sorgente);
    fclose(dizionario);
    fclose(ricodificato);


    return 0;
}


int leggiDizionario (Dizionario d[], FILE *dizionario){
    int S,id;
    char originale[M];
    char tmp[M];

    if(fscanf(dizionario,"%d",&S)== 1){ //implicito. controllo assolto nella sezione check input
     printf("Dizionario contiene %d ricodifiche\n",S);
        int count = 0;

    while (!feof(dizionario)){
          if (fscanf(dizionario,"%s %s",tmp,originale) == 2){ //se leggo oltre la fine del file tramite feof scanf ritorna 0
           sscanf(tmp,"$%d$",&id);
           // -----------------------------
           strcpy(d[count].originale,originale);
           d[count].id = id;
           d[count].l = strlen(d[count].originale);
            count++;
            //printf("%d \n",id);
          }
        }

    if(count == S ) return S;
    else return -1; //errore generico

    }

    else return -1; //errore generico

}

void ricodifica (Dizionario d[], FILE* sorgente, FILE* out, int S){

    int i,j,ii,jj,m,n,k,indice,finito=0;
    char tmp_str[N];
    char tmp_start[M];
    char tmp_end[M];
    char *ps;
    // char tmp[M]; OTTIMIZZATO

    j=0;
    ii =0;

    while(fgets(tmp_str,N,sorgente) != NULL){

        j=0;
        for(i=0; i<strlen(tmp_str)+1; i++){ //RIGA

            if(tmp_str[i] == ' ' || i == strlen(tmp_str)-1 ){
            /*Trovando sottostringhe evito inutili strcmp() */
                finito =0;
                ii = 0;
                for(jj=i-j; jj<i; jj++){
                    tmp_start[ii]=tmp_str[jj];
                    ii = ii+1;
                }

                tmp_start[ii]='\0'; //ii == strlen(tmp_start)
                strcpy(tmp_end,tmp_start); //evito modifiche sulla stringa (tolower)

                for(m=0; m<ii; m++){
                    if(tmp_start[m]<'a')
                        tmp_start[m] = tolower(tmp_start[m]);
                }

                printf("--- PAROLA: %s ---\n",tmp_start);

                    for(k=0; k<S && !finito; k++){ //Verifica dizionario

                     printf("---Provo con %s \n\n",d[k].originale);

                        if(d[k].l<=ii ){ //Verifica compatibilità

                         for(m=0; m<ii && !finito; m++){//Appena trovo una sostituzione finito = 1


                                /*  Rif. pag 50 Dal problema al programma
                                strncmp(); => valuta i primi n caratteri
                                ps = &(tmp_start[m]); => ps è il puntatore al carattere i-esimo della stringa
                                Per cui evito di salvare ulteriore stringa in memoria !

                                char tmp[d[k].l];
                                int mm;

                                for(mm=0;  mm<d[k].l ; mm++){ //mm<d[k].l controllo assolto dal precedente
                                    tmp[mm] = tmp_start[m+mm];

                                }
                                tmp[d[k].l] = '\0'; //chiudo la stringa */

                                ps = &(tmp_start[m]);

                                if(strlen(ps)>=d[k].l){ //Evito cmp inutili
                                    printf("%s \n\n",ps);
                                    if(! strncmp(d[k].originale,ps,d[k].l)) {
                                        printf("OK !\n");
                                        finito = 1;
                                        indice = k;
                                    }
                               }

                         }

                      }

                    }

                printf("Parola %s ricodificata (SI != 0): %d \n\n",tmp_start,finito);

                if(!finito){
                        fprintf(out,"%s",tmp_end);
                }

                else{
                    for(n=0; n<m-1; n++)fprintf(out,"%c",tmp_end[n]);
                    fprintf(out,"$%d$",d[indice].id);

                        if((n+d[indice].l)<ii){
                            for(n=n+d[indice].l; n<ii; n++)fprintf(out,"%c",tmp_end[n]);
                        }
                }

                j=0;
                if(i != strlen(tmp_str)-1)fprintf(out," ");
            }


            else j=j+1;

      }

        fprintf(out,"\n");
        //printf("\n");
    }

}





