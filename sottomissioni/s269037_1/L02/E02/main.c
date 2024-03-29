#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGFILE "log.txt"
#define N 1000
#define S 30+1
#define SS 300+1


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
r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine,r_errore
}e_comandi;

e_comandi leggiComando(void);
e_comandi Date (Corsa c[], int n);
e_comandi Partenza (Corsa c[], int n);
e_comandi Capolinea (Corsa c[], int n);
e_comandi Ritardo (Corsa c[], int n);
e_comandi RitardoTot (Corsa c[], int n);
void toMinuscolo(char str[]);
int creaCorse(Corsa corse[]);
void selezionaDati(e_comandi c, Corsa corse[], int n);
void toMaiuscolo(char str[]);
int testDate(char d1[],char d2[],char d[]);
int verificaFormato(char date[]);
void stampaTratta(Corsa c);


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
        printf("Inserire comando come da specifiche sopra indicate: ");

         if(scanf("%s",cmd)==1){
            toMinuscolo(cmd);
            c = r_date;
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
    /* Provo ulteriore applicazione enum sulla visualizzazione errori*/
    char c_tabella_Errori[r_errore][SS]= {
    "Intervallo date mancante o non corretto",
    "Stazione di partenza mancante o non corretta",
    "Capolinea mancante o non corretto",
    "Intervallo date mancante o non corretto",
    "Codice tratta mancante o non corretta"
    };
    e_comandi cod=r_errore;
        switch (c) {
            case r_date: cod = Date(corse,n);
             break;
            case r_partenza: cod = Partenza(corse,n);
             break;
            case r_capolinea: cod = Capolinea(corse,n);
             break;
            case r_ritardo: cod = Ritardo(corse,n);
             break;
            case r_ritardo_tot: cod =  RitardoTot(corse,n);
             break;
            default:exit(-1);
        }

        /*Gli ulteriori parametri del comando sono acquisiti in lettura da
        stdin*/

    if(cod==r_errore)printf("Errore: %s\n",c_tabella_Errori[c]);

return;
}

e_comandi Date (Corsa c[], int n){
    e_comandi cod= r_errore;
    int i;
    char data1[S],data2[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s %s",data1,data2)==2 && verificaFormato(data1) && verificaFormato(data2)){
      if(strcmp(data1,data2) <= 0){
            cod = r_date;
            printf("Corse dal %s al %s: \n",data1,data2);
            for(i=0; i<n; i++){
                if(testDate(data1,data2,c[i].date))
                   stampaTratta(c[i]);
            }
      }
    }
    return cod;
}
e_comandi Partenza (Corsa c[], int n){
    e_comandi cod= r_errore;
     char fermata[S];
     char fermataTest[S],cmd[S];
     int i;
    gets(cmd);
    if(sscanf(cmd,"%s",fermata)==1)
    {
        cod = r_partenza;
        printf("Corse da %s\n",fermata);
        toMinuscolo(fermata);
        for(i=0; i<n; i++){
          strcpy(fermataTest,c[i].start);
          toMinuscolo(fermataTest);
            if(!strcmp(fermataTest,fermata))
                stampaTratta(c[i]);
        }

    }
    return cod;
}
e_comandi Capolinea (Corsa c[], int n){
    e_comandi cod= r_errore;
     char fermata[S],fermataTest[S],cmd[S];
     int i;
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",fermata)==1)
    {
        cod = r_capolinea;
        printf("Corse verso %s\n",fermata);
        toMinuscolo(fermata);
        for(i=0; i<n; i++){
           strcpy(fermataTest,c[i].end);
           toMinuscolo(fermataTest);
            if(!strcmp(fermataTest,fermata))
                stampaTratta(c[i]);
        }

    }
    return cod;
}
e_comandi Ritardo (Corsa c[], int n){
    e_comandi cod= r_errore;
    int i;
    char data1[S],data2[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */

    if(sscanf(cmd,"%s %s",data1,data2)==2 && verificaFormato(data1) && verificaFormato(data2))
    {
        if(strcmp(data1,data2) <= 0){
            cod = r_ritardo;
            printf("Corse in ritardo dal %s al %s: \n",data1,data2);
            for(i=0; i<n; i++){
                if(testDate(data1,data2,c[i].date) && c[i].delay_time)
                    stampaTratta(c[i]);
            }
        }

    }
    return cod;
}
e_comandi RitardoTot (Corsa c[], int n){
    e_comandi cod= r_errore;
    int i,r=0;
    char tratta[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */

    if(sscanf(cmd,"%s",tratta)==1)
    {
        cod = r_ritardo_tot;
        toMaiuscolo(tratta);
        //printf("Corse in ritardo codice tratta %s: \n",tratta);
        for(i=0; i<n; i++){
            if(!strcmp(c[i].codTratta,tratta)&& c[i].delay_time){
                //stampaTratta(c[i]);
                r = r+c[i].delay_time;
            }
        }
        printf("RITARDO TOTALE PER %s: %d MINUTI\n\n",tratta,r);
    }
    return cod;
}

int testDate(char d1[],char d2[],char d[]){
    /*verifico se d1<=d<=d2 */
    if(strcmp(d1,d)<=0 && strcmp(d2,d)>=0)
        return 1;
return 0;
}

int verificaFormato(char date[]){
    /*formato data: YYYY/MM/DD
    Ricodifico date se espresse nel formato DD/MM/YYYY */
    char y[S],m[S],d[S];
    int i;

    for(i=0; i<strlen(date); i++)
        if(date[i]=='/')
            date[i]=' ';

    if(sscanf(date,"%s %s %s",y,m,d)==3){

        if(strlen(y)==4 && strlen(m)==2 && strlen(d)==2)
            return 1;
        else if (strlen(y)==2 && strlen(m)==2 && strlen(d)==4){
            sprintf(date,"%s/%s/%s",d,m,y);/*sono certo che la stringa non varia la sua dimensione */
        }

        for(i=0; i<strlen(date); i++)
        if(date[i]==' ')
            date[i]='/';

            return 1;
    }
return 0;
}


void stampaTratta(Corsa c){
    printf("\n");
    printf("%s da %s a %s del %s PARTENZA: %s ARRIVO: %s RITARDO: %d %s\n\n",c.codTratta,c.start,
        c.end,c.date,c.start_time,c.end_time,c.delay_time,c.delay_time==1?"minuto":"minuti");
 return;
}

