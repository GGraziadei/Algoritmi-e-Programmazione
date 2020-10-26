#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGFILE "log.txt"
#define N 1000
#define S 30+1
#define SS 300+1

#define SCHERMO "stdout"


typedef struct{
int id;
int delay_time;
char codTratta[S];
char start[S];
char end[S];
char start_time[S];
char end_time[S];
char date[S];
/*Tramite il formato data: YYYY/MM/DD � possibile utilizzare la strcmp per confrontare date*/
}Corsa;

typedef enum {
r_stampa, r_ordina_time, r_ordina_tratta, r_ordina_partenza, r_ordina_arrivo,r_ricerca_partenza, r_fine,r_errore
}e_comandi;

e_comandi stampa (Corsa c[], int n);
e_comandi leggiComando(void);
void toMinuscolo(char str[]);
int creaCorse(Corsa corse[]);
void selezionaDati(e_comandi c, Corsa corse[], int n);
void toMaiuscolo(char str[]);
int testDate(char d1[],char d2[],char d[]);
int verificaFormato(char date[]);
void stampaTratta(FILE *out,Corsa c);


int main()
{
    Corsa corse[N];
    e_comandi CODcomando;
    int finito = 0;
    int n = creaCorse(corse);


    while(!finito){
        CODcomando = leggiComando();
        if(CODcomando != r_fine)
         selezionaDati (CODcomando,corse,n);
        else
         finito =1;
    }

    return 0;
}



e_comandi leggiComando(void){
    int i;
    char cmd[S];
    e_comandi c = r_errore;
    char c_tabella[r_errore][S]={
     "stampa","ordina_time","ordina_tratta","ordina_partenza","ordina_arrivo","ricerca_partenza","fine"
    };
    char c_tabella_Comandi [r_errore][SS]={
     "1. Stampa log: stampa <nome_file/stdout> ATTENZIONE SE STAMPI SU FILE INSERISCI ESTENSIONE",
     "2. ordinamento del vettore per data, e a parit� di date per ora: ordina_time",
     "3. ordinamento del vettore per codice di tratta: ordina_tratta",
     "4. ordinamento del vettore per stazione di partenza: ordina_partenza",
     "5. ordinamento del vettore per stazione di arrivo: ordina_arrivo",
     "6. ricerca di una tratta per stazione di partenza: ricerca_partenza <stazione partenza (anche stringa parziale)>",
     "7. Termina esecuzione: fine"
    };

    do{

        printf("---------- MENU DI SELEZIONE ----------\n");
        for(i=0; i<r_errore; i++)printf("%s \n",c_tabella_Comandi[i] );
        printf("Inserire comando come da specifiche sopra indicate: ");

         if(scanf("%s",cmd)==1){
            toMinuscolo(cmd);
            c = r_stampa;
            while(c<r_errore && strcmp(c_tabella[c],cmd)!=0 ) c++;
         }
         if(c==r_errore){
                printf("Comando non valido\n\n");
                fgets(cmd,S,stdin); /*fine lettura comando precedente,
                predispongo cmd per ricevere un nuovo comando.*/

         }
    }while(c == r_errore);

    return c;

};


void toMinuscolo(char str[]){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]<'a')
         str[i] = tolower(str[i]);
 return;
}

void toMaiuscolo(char str[]){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]>'a')
         str[i] = toupper(str[i]);
 return;
}

int creaCorse(Corsa corse[]){
    int i,n=0;
    FILE *fp = fopen(LOGFILE,"r");
    if(fp == NULL)exit(-1);

    fscanf(fp,"%d",&n);
        for(i=0; i<n; i++){
            corse[i].id  = i;
            corse[i].delay_time = 0;
            fscanf(fp,"%s %s %s %s %s %s %d",corse[i].codTratta,corse[i].start,
                   corse[i].end,corse[i].date,corse[i].start_time,corse[i].end_time,&corse[i].delay_time);
        }

    fclose(fp);
    return n;
}

void selezionaDati(e_comandi c, Corsa corse[], int n){

    e_comandi cod=r_errore;
        switch (c) {
            case r_stampa: cod = stampa(corse,n);
             break;
            /*case r_partenza: cod = Partenza(corse,n);
             break;
            case r_capolinea: cod = Capolinea(corse,n);
             break;
            case r_ritardo: cod = Ritardo(corse,n);
             break;
            case r_ritardo_tot: cod =  RitardoTot(corse,n);
             break;*/
            default:exit(-1);
        }

        /*Gli ulteriori parametri del comando sono acquisiti in lettura da
        stdin*/

    if(cod==r_errore)printf("Errore\n");

return;
}

e_comandi stampa (Corsa c[], int n){
    e_comandi cod= r_errore;
    int i;
    char fileout[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",fileout)==1 ){
        toMinuscolo(fileout);
        cod = r_stampa;
      if(strcmp(fileout,SCHERMO) == 0)
        stampaSchermo(c,n);
      else
        stampaFile(c,n,fileout);
    }
    return cod;
}




void stampaSchermo (Corsa c[],int n){
    int i=0;
    for(i=0; i<n; i++)
        stampaTratta(stdout,c[i]);
return;
}

void stampaFile (Corsa c[],int n, char *fileout){
    FILE *out = fopen(fileout,"w");
    int i=0;
    for(i=0; i<n; i++)
        stampaTratta(out,c[i]);
    fclose(out);
return;
}

void stampaTratta(FILE *out,Corsa c){
    printf("\n");
    fprintf(out,"%s da %s a %s del %s PARTENZA: %s ARRIVO: %s RITARDO: %d %s\n\n",c.codTratta,c.start,
        c.end,c.date,c.start_time,c.end_time,c.delay_time,c.delay_time==1?"minuto":"minuti");
 return;
}

