#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGFILE "log.txt"
#define N 1000
#define S 30+1
#define SS 300+1
#define SHOW_SWAP 0

#define SCHERMO "schermo"

typedef enum {
    r_stampa, r_ordina_time, r_ordina_tratta, r_ordina_partenza, r_ordina_arrivo,r_ricerca_partenza, r_fine,r_errore
}e_comandi;

typedef enum{
    codTratta,start,end,time,errore
}e_key;

typedef struct {
    int a;
    int m;
    int g;
} data_t;

typedef struct {
    int h;
    int m;
    int s;
} ora_t;

typedef struct{
    int delay_time;
    char codTratta[S];
    char start[S];
    char end[S];
    char start_time[S];
    char end_time[S];
    char date[S];
    data_t Gd;
    ora_t Hp, Ha;
}Corsa;

typedef struct {
    int n;
    Corsa corse[N];
    e_key chiaveOrdinamento;
} tabella_t;

tabella_t leggiLog();
e_comandi leggiComando(char c_tabella[][S]);
e_comandi wrapper_ricercaPartenza (tabella_t *tab);
e_comandi wrapper_stampa (tabella_t *tab);
e_comandi  wrapper_ordina (tabella_t *tab,e_key chiave);
void toMinuscolo(char str[]);
void selezionaDati(e_comandi c, tabella_t *tab );
void toMaiuscolo(char str[]);
void stampaTratta(FILE *out,Corsa c);
void stampaFile (tabella_t *tab, char *fileout);
void stampaSchermo (tabella_t *tab);
void  scambiaCorsa(tabella_t *tab, int indice );
void Ordina (tabella_t *tab,e_key chiave);
int linearSearch (tabella_t *tab, char *s);
int binarySearch (tabella_t *tab, char *s, int inf, int sup);
int confrontaStr (char *s1,char *s2, int l);
int confrontaTime (Corsa item1, Corsa item2 );


int main() {
    /*Come da specifiche acquisite su Slack al contrario dell'esercizio 5 ordino direttamente in loco sul vettore corse
     * questo implica l'impossibilità di mantenere in simultanea più ordinamenti. Utilizzando last_update tab.chiaveOrdinamento
     * momorizza l'ultima chiave secondo cui si è ordinato il vettore*/
    tabella_t tab;
    e_comandi CODcomando;
    int finito = 0;
    char c_tabella[r_errore][S]={ "stampa","ordina_time","ordina_tratta","ordina_partenza","ordina_arrivo","ricerca_partenza","fine" };
    tab = leggiLog();
    while(!finito){
        CODcomando = leggiComando(c_tabella);
        if(CODcomando != r_fine)
            selezionaDati (CODcomando,&tab);
        else
            finito =1;
    }
    return 0;
}


tabella_t leggiLog(){
    tabella_t tab;
    int i;
    FILE *fp = fopen(LOGFILE,"r");
    if(fp == NULL)exit(-1);
    fscanf(fp,"%d",&tab.n);
    tab.chiaveOrdinamento = errore;

    for(i=0; i<tab.n; i++){
        tab.corse[i].delay_time = 0;
        fscanf(fp,"%s %s %s %s %s %s %d",tab.corse[i].codTratta,tab.corse[i].start,
               tab.corse[i].end,tab.corse[i].date,tab.corse[i].start_time,tab.corse[i].end_time,&tab.corse[i].delay_time);
        sscanf(tab.corse[i].start_time,"%d:%d:%d",&tab.corse[i].Hp.h,&tab.corse[i].Hp.m,&tab.corse[i].Hp.s);
        sscanf(tab.corse[i].end_time,"%d:%d:%d",&tab.corse[i].Ha.h,&tab.corse[i].Ha.m,&tab.corse[i].Ha.s);
        sscanf(tab.corse[i].date,"%d/%d/%d",&tab.corse[i].Gd.a,&tab.corse[i].Ha.m,&tab.corse[i].Ha.s);

        //printf("%s \n",tab.toCorsa[i]->codTratta);
    }

    fclose(fp);
    return tab;
}

e_comandi leggiComando(char c_tabella[][S]){
    int i;
    char cmd[S];
    e_comandi c = r_errore;

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
            gets(cmd); /*fine lettura comando attuale,
                predispongo cmd per ricevere un nuovo comando.*/
        }
    }while(c == r_errore);

    return c;

}

void selezionaDati(e_comandi c, tabella_t *tab ){
/*Comandi disponibili
r_stampa, r_ordina_time, r_ordina_tratta, r_ordina_partenza, r_ordina_arrivo,r_ricerca_partenza, r_fine,r_errore*/
    e_comandi cod;

    switch (c) {
        case r_stampa: cod = wrapper_stampa(tab);
            break;
        case r_ricerca_partenza: cod = wrapper_ricercaPartenza(tab);
            break;
        case r_ordina_time: cod = wrapper_ordina(tab,time);
            break;
        case r_ordina_partenza: cod = wrapper_ordina(tab,start);
            break;
        case r_ordina_arrivo: cod = wrapper_ordina(tab,end);
            break;
        case r_ordina_tratta: cod = wrapper_ordina(tab,codTratta);
            break;
        default:exit(-1);
    }

    /*Gli ulteriori parametri del comando sono acquisiti in lettura da
    stdin*/

    if(cod != c)printf("Errore\n");

    return;
}

e_comandi wrapper_stampa (tabella_t *tab){
    e_comandi cod= r_errore;

    char fileout[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",fileout)==1 ){
        toMinuscolo(fileout);
        cod = r_stampa;
        if(strcmp(fileout,SCHERMO) == 0)
            stampaSchermo(tab);
        else
            stampaFile(tab,fileout);
    }
    return cod;
}

e_comandi wrapper_ricercaPartenza (tabella_t *tab){
    e_comandi cod= r_errore;
    int idCorsa,i,stampa=1;
    char search[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",search)==1 ){
        cod = r_ricerca_partenza;
       if(tab->chiaveOrdinamento == start){
           printf("\nBINARY SEARCH\n");
           if(SHOW_SWAP)stampaSchermo(tab);
           idCorsa = binarySearch(tab,search,0,tab->n-1);
           /*Binary search attraverso una lieve modifica della ricerca binaria ricorsiva
             ritorna l'id al primo elemento corrispondente ai criteri di ricerca (id rif vettore dei puntatori)
           Se non trova corrispondenze ritorna -1 */
           if (idCorsa >= 0) {
               for (i = idCorsa;  i >= 0 && stampa && i < tab->n; i++) {
                   if (confrontaStr(tab->corse[i].start, search, strlen(search)) == 0)
                       stampaTratta(stdout, tab->corse[i]);
                   else stampa = 0;
               }
           }
           else printf("Nessuna occorrenza riscontrata per %s",search);
       }
       else {
           printf("\nLINEAR SEARCH\n");
           idCorsa = linearSearch(tab, search);
           if (idCorsa<1) printf("Nessuna occorrenza riscontrata per %s",search);
       }
    }
    return cod;
}

e_comandi  wrapper_ordina (tabella_t *tab,e_key chiave){
    Ordina(tab,chiave);
    tab->chiaveOrdinamento = chiave;
    e_comandi  cod = r_errore;
    switch (chiave) {
        case time: cod = r_ordina_time;
            break;
        case codTratta : cod = r_ordina_tratta;
            break;
        case start : cod = r_ordina_partenza;
            break;
        case end : cod = r_ordina_arrivo;
            break;
        default: cod=r_errore;
    }
    return cod;
}

int linearSearch (tabella_t *tab, char *s){
    int i=0,conta=0;
    for(i=0; i<tab->n; i++){
        if( confrontaStr(tab->corse[i].start, s, strlen(s)) ==0) {
            stampaTratta(stdout, tab->corse[i]);
            conta = conta+1;
        }
    }
    return conta;
}

int binarySearch (tabella_t *tab, char *s, int inf, int sup) {
    if(sup-inf == 0) {
        if (confrontaStr (tab->corse[inf].start, s, strlen(s) ) == 0)
            return inf;
        else
            return -1;
    }

    int m = (sup+inf)/2;
    if (confrontaStr (tab->corse[m].start, s, strlen(s)) >=0)
        return binarySearch(tab,s,inf,m);
    else
        return binarySearch(tab,s,m+1,sup);

}

void Ordina (tabella_t *tab,e_key chiave){
    /*Bubble sort Algoritmo stabile */
    int i,j,res;
    for(i=0; i<tab->n; i++){
        for(j=0;j<tab->n-i-1; j++){
            switch (chiave) {
                case time:
                    res = confrontaTime(tab->corse[j], tab->corse[j + 1]);
                    break;
                case codTratta :
                    res = confrontaStr(tab->corse[j].codTratta, tab->corse[j + 1].codTratta, 0);
                    break;
                case start :
                    res = confrontaStr(tab->corse[j].start, tab->corse[j + 1].start, 0);
                    break;
                case end :
                    res = confrontaStr(tab->corse[j].end, tab->corse[j + 1].end, 0);
                    break;
                default:
                    res = -1;
            }
            if(res>0){
                if(SHOW_SWAP){
                    stampaTratta(stdout,tab->corse[j]);
                    stampaTratta(stdout,tab->corse[j+1]);
                    printf("------------SCAMBIA-------------");
                }
                scambiaCorsa(tab,j);
            }

        }
    }

    return;
}
int confrontaTime (Corsa item1, Corsa item2 ){
    /*Copio localmente le due corse da analizzare*/
    int res = confrontaStr(item1.date,item2.date,0);
    if(res == 0){
        res = confrontaStr(item1.start_time,item2.start_time,0);
            if(res == 0)
                res = confrontaStr(item1.end_time,item2.end_time,0);
    }
    return  res;
}

void  scambiaCorsa(tabella_t *tab, int indice ){
    Corsa tmp = tab->corse[indice];
    tab->corse[indice]= tab->corse[indice+1];
    tab->corse[indice+1] = tmp;
    return;
}

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

int confrontaStr (char *s1,char *s2, int l){
    char item1[S],item2[S];
    strcpy(item1,s1);
    strcpy(item2,s2);
    toMaiuscolo(item1);
    toMaiuscolo(item2);
    if(l !=0 )
        return strncmp(item1,item2,l);
    else
        return strcmp(item1,item2);
}

void stampaSchermo (tabella_t *tab){
    int i=0;
    for(i=0; i<tab->n; i++)
        stampaTratta(stdout,tab->corse[i]);
    return;
}

void stampaFile (tabella_t *tab, char *fileout){
    FILE *out = fopen(fileout,"w");
    int i;
    for(i=0; i<tab->n; i++)
        stampaTratta(out,tab->corse[i]);
    fclose(out);
    return;
}

void stampaTratta(FILE *out,Corsa c){
    /*Ricevuti in input corse e file di output stampa*/
    printf("\n");
    fprintf(out,"%s da %s a %s del %s PARTENZA: %s ARRIVO: %s RITARDO: %d %s\n\n",c.codTratta,c.start,
            c.end,c.date,c.start_time,c.end_time,c.delay_time,c.delay_time==1?"minuto":"minuti");
    return;
}