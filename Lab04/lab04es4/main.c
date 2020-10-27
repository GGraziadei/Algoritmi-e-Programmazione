#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGFILE "log.txt"
#define N 1000
#define S 30+1
#define SS 300+1

#define SCHERMO "schermo"


typedef struct{
int id;
int delay_time;
char codTratta[S];
char start[S];
char end[S];
char start_time[S];
char end_time[S];
char date[S];
/*Tramite il formato data: YYYY/MM/DD è possibile utilizzare la strcmp per confrontare date*/
}Corsa;

typedef enum {
r_stampa, r_ordina_time, r_ordina_tratta, r_ordina_partenza, r_ordina_arrivo,r_ricerca_partenza, r_fine,r_errore
}e_comandi;

typedef enum{
id,delay_time,codTratta,start,end,start_time,end_time,date,errore
}e_key;

e_comandi stampa (Corsa c[], int n);
e_comandi leggiComando(void);
void toMinuscolo(char str[]);
int creaCorse(Corsa corse[]);
e_key selezionaDati(e_comandi c, Corsa corse[], int n, e_key ordinato);
e_comandi ricercaPartenza (Corsa c[], int n, e_key *chiave);
void toMaiuscolo(char str[]);
int testDate(char d1[],char d2[],char d[]);
int verificaFormato(char date[]);
void stampaTratta(FILE *out,Corsa c);


int main()
{
    Corsa corse[N];
    e_comandi CODcomando;
    e_key isOrderByKey = errore; /*Stato attuale di ordinamento*/
    int finito = 0;
    int n = creaCorse(corse);


    while(!finito){
        CODcomando = leggiComando();
        if(CODcomando != r_fine)
         isOrderByKey = selezionaDati (CODcomando,corse,n,isOrderByKey);
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
     "1. Stampa log: stampa <nome_file/schermo> ATTENZIONE SE STAMPI SU FILE INSERISCI ESTENSIONE",
     "2. ordinamento del vettore per data, e a parità di date per ora: ordina_time",
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

e_key selezionaDati(e_comandi c, Corsa corse[], int n, e_key ordinato){

    e_comandi cod=r_errore;

        switch (c) {
            case r_stampa: cod = stampa(corse,n);
             break;
            case r_ricerca_partenza: cod = ricercaPartenza(corse,n,&ordinato);
             break;
            /*case r_capolinea: cod = Capolinea(corse,n);
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

return ordinato;
}

e_comandi stampa (Corsa c[], int n){
    e_comandi cod= r_errore;

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

e_comandi ricercaPartenza (Corsa c[], int n, e_key *chiave){
    e_comandi cod= r_errore;
    char search[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",search)==1 ){
        cod = r_ricerca_partenza;
        wrapper_binarySearch(c,n,start,*chiave,search);
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

void wrapper_binarySearch(Corsa c[],int n,e_key chiave, e_key chiaveAttuale, char search[]){
    int i,id = -1,stampa=1;
    if(chiaveAttuale==chiave || isOrderBy(chiave,c,n)){
     id = binarySearchS(c,0,n-1,search);
     /*Binary search ritorna l'id al primo elemento corrispondente ai criteri di ricerca
     Se non trovato ritorna -1;*/
     for(i=id;i<n && i>=0 && stampa; i++){
        if(confrontaNS(c[i].start,search,strlen(search)))
            stampaTratta(stdout,c[i]);
        else stampa = 0;
     }
    }
    else
        linearSearchS (c,n,search);


return;
}

int binarySearchS (Corsa c[],int inf,int sup,char s[]){
    /*Ricerca binaria su stringhe*/
    if(!(sup-inf)){
        if(confrontaNS(c[inf].start,s,strlen(s))==0)
            return inf;
        else
            return -1;
    }

    int m = (sup+inf)/2;

    if(confrontaNS(c[m].start,s,strlen(s)) >= 0)
        binarySearchS(c,inf,m,s);
    else
        binarySearchS(c,m+1,sup,s);

}

void linearSearchS (Corsa c[],int n,char s[]){
    int i;
    char test[S];

    for(i=0;i<n; i++){
        if(confrontaNS(c[i].start,s,strlen(s))==0)
            stampaTratta(stdout,c[i]);
    }

return;
}
int isOrderBy(e_key chiave,Corsa c[],int n){
    /* Verifca ordinamento rispetto generica chiave.
    Valida solo per chiavi di tipo vettore carattere
    chiavi disponibili: codTratta,start,end,start_time,end_time,date*/
     int i=0,ordinato=1,res;

    for(i=0; i<n-1 && ordinato; i++){
      switch (chiave){
        case codTratta:res = confrontaS(c[i].codTratta,c[i+1].codTratta);
         break;
        case start:res = confrontaS(c[i].start,c[i+1].start);
         break;
        case end:res = confrontaS(c[i].end,c[i+1].end);
         break;
        case start_time:res = confrontaS(c[i].start_time,c[i+1].start_time);
         break;
        case end_time:res = confrontaS(c[i].end_time,c[i+1].end_time);
         break;
        case date:res = confrontaS(c[i].date,c[i+1].date);
         break;
        default : res=-1;
      }
      if(res>0) ordinato=0;
    }
    return ordinato?chiave:-1;
}

int confrontaNS (char *s1,char *s2, int n){
    char item1[S],item2[S];
    strcpy(item1,s1);
    strcpy(item2,s2);
    toMinuscolo(item1);
    toMinuscolo(item2);
    return strncmp(s1,s2,n);
}

int confrontaS (char *s1,char *s2){
    char item1[S],item2[S];
    strcpy(item1,s1);
    strcpy(item2,s2);
    toMinuscolo(item1);
    toMinuscolo(item2);
    return strcmp(s1,s2);
}
