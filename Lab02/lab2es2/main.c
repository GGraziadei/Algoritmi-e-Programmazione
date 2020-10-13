#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGFILE "log.txt"
#define N 1000
#define S 30+1
#define SS 300+1
#define ANNO_START 2000

typedef struct{
int id;
char codTratta[S];
char start[S];
char end[S];
//char start_time[S];
//char end_time[S];  La risoluzione del problema non richiede queste informazioni
int delay_time;
char date[S];
/*Tramite il formato
data è possibile utilizzare la strcmp per confrontare date*/
}Corsa;

typedef enum {
r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine,r_errore
}e_comandi;

e_comandi leggiComando(void);

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
     "date","partenza","capolinea","ritardo","ritardo_tot","fine"
    };
    char c_tabella_Comandi [r_errore][SS]={
     "1. elenca tutte le corse partite in un certo intervallo di date: date <data1> <data2>",
     "2. elenca tutte le corse partite da una certa fermata: partenza <stazione_partenza>",
     "3. elenca tutte le corse che fanno capolinea ad una certa fermata: capolinea <stazione_capolinea>",
     "4. elenca tutte le corse che hanno registrato ritardo in un intervallo di date: ritardo <data1> <data2>",
     "5. ritardo complessivo accumulato dalle corse di un dato codice tratta: ritardo_tot <codice_tratta>",
     "6. Termina esecuzione: fine"
    };

    do{
        printf("---------- MENU DI SELEZIONE ----------\n");
        for(i=0; i<r_errore; i++)printf("%s \n",c_tabella_Comandi[i] );
        printf("Inserire comando valido come da specifiche: ");

         if(scanf("%s",cmd)==1){
            toMinuscolo(cmd);
            c = r_date;
            while(c<r_errore && strcmp(c_tabella[c],cmd) ) c++;
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

int creaCorse(Corsa corse[]){
    int i,n=0;
    FILE *fp = fopen(LOGFILE,"r");
    if(fp == NULL)exit(-1);

    fscanf(fp,"%d",&n);
        for(i=0; i<n; i++){
            corse[i].id  = i;
            corse[i].delay_time = 0;
            fscanf(fp,"%s %s %s %s %*s %*s %d",corse[i].codTratta,corse[i].start,
                   corse[i].end,corse[i].date,&corse[i].delay_time); /*Informazioni ora partenza ora arrivo non richieste, le salto in lettura*/
        }

    fclose(fp);
    return n;
}

void selezionaDati(e_comandi c, Corsa corse[], int n){

    char str1[S],str2[S];
    n = scanf("%d %d",str1,str2);
    char info[r_errore][SS]= {
    "Inserire intervallo date separate da spazio: ",
    "Inserire fermata di partenza: ",
    "Inserire fermata capolinea: ",
    "Inserire intervallo date separate da spazio: "
    };

        switch (c) {
            case r_date: printf("data");
             break;
            default: printf("Errore comando non valido \n");
        }



return;
}


