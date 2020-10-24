#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 200+1
#define M 100+1
#define S_MAX 30
#define KEYSENSITIVE_RICODIFICA 0/*Se non attivo riconosce la sequenza "SEQ" in "sequenza"
Ho testato modificando in maiuscolo APOLLO in sorgente.txt*/


typedef struct Dizionario{
int id;
int l;
char originale[M];

}Dizionario;

int ricodifica (Dizionario d[], FILE* sorgente, FILE* out, int S);
int leggiDizionario (Dizionario d[], FILE *dizionario);
void toMinuscolo(char c[]);

int main()
{
    FILE* sorgente = fopen("sorgente.txt","r");
    FILE* dizionario = fopen("dizionario.txt","r");
    FILE* ricodificato = fopen("ricodificato.txt","w");
    int S,r,dizionarioCheck = 1; //Dizionario presente
    Dizionario d[S_MAX];


    // ----------------------------- verifica input
    if(sorgente == NULL){
        printf("Inserire FILE sorgente");
        return -1;
    }
    if (dizionario == NULL){
        printf("FILE dizionario mancante\n");
        dizionarioCheck = 0;
    }

    if(dizionarioCheck){
            S = leggiDizionario(d, dizionario); // S  numero di words in dizionario
            if(S > S_MAX) {
                printf("Errore formato Dizionario");
                exit(-1);
            }
            else if(S != -1) printf("Lettura dizionario completata correttamente \n");

            r = ricodifica(d,sorgente,ricodificato,S);
            printf("%d ricodifiche effettuate",r);
    }


    fclose(sorgente);
    fclose(dizionario);
    fclose(ricodificato);


    return 0;
}


int leggiDizionario (Dizionario d[], FILE *dizionario){
    int S,id;
    char originale[M];
    char tmp[M];

    if(fscanf(dizionario,"%d",&S)== 1){
     printf("Dizionario contiene %d ricodifiche\n",S);
        int count = 0;

    while (!feof(dizionario)){
          if (fscanf(dizionario,"%s %s",tmp,d[count].originale) == 2){
           sscanf(tmp,"$%d$",&id);
           // -----------------------------
           if(!KEYSENSITIVE_RICODIFICA)toMinuscolo(originale);
           d[count].id = id;
           d[count].l = strlen(d[count].originale);
            count++;
          }
        }

    if(count == S ) return S;
    else return -1; //errore generico

    }

    else return -1; //errore generico

}

int ricodifica (Dizionario d[], FILE* sorgente, FILE* out, int S){

    int  i,k,ii,indice,trovato=0,r=0;
    char tmp_str[N];
    char tmp_end[N];
    char *ps;

    while(fgets(tmp_str,N,sorgente) != NULL){//RIGA
        strcpy(tmp_end,tmp_str);
        i = 0; //leggo carattere per carattere
        ii = strlen(tmp_str);
        if(!KEYSENSITIVE_RICODIFICA) toMinuscolo(tmp_str);
        while(i<ii){

          trovato = 0;
          for(k=0; k<S && !trovato; k++){ //Verifica dizionario
                if(tmp_str[i]==d[k].originale[0] && (ii - i)>=d[k].l){ //Verifica compatibilitÓ
                    ps = &(tmp_str[i]);
                    if(! strncmp(d[k].originale,ps,d[k].l)){
                        trovato = d[k].l; //Ovvero la lunghezza della sostituzione
                        indice = k;
                        r++;
                    }
                }
          }
          if(!trovato){
            fprintf(out,"%c",tmp_end[i]);
            i = i+1;
          }
          else{
            fprintf(out,"$%d$",d[indice].id);
            i = i+trovato; //Salvo in trovato la lunghezza della sostituzione
           }
        }
      //Fine riga
    }
return r;
}

void toMinuscolo(char c[]){
int m;
for(m=0; m<strlen(c); m++){
    if(c[m]<'a')
     c[m] = tolower(c[m]);
}
return;
}





