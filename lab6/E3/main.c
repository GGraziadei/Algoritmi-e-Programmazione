#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LOGFILE "log.txt"
/* #define N 1000 l'obiettivo è allocare dinamicamente */
#define S 30+1
#define SS 300+1
#define SHOW_SWAP 0
#define SCODTRATTA 6
#define SCHERMO "schermo"

typedef enum {
    r_stampa, r_ordina_time, r_ordina_tratta, r_ordina_partenza, r_ordina_arrivo,r_ricerca_partenza,r_ricerca_arrivo,r_ricerca_date,r_ricerca_codTratta, r_ritardo,r_ritardo_tot,r_date,r_carica_log,r_fine,r_errore
}e_comandi;

typedef enum{
    codTratta,start,end,time,errore
}e_key;

typedef  enum {
    esatta,precede,segue
}e_searchType;

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
typedef struct {
    int idInf;
    int idSup;
}IntervalloId;
typedef struct{
    int delay_time;
    char *codTratta;
    char *start;
    char *end;
    char *start_time;
    char *end_time;
    char *date;
    data_t Gd;
    ora_t Hp, Ha;
}Corsa;
typedef struct {
    e_key chiaveOrdinamento;
    Corsa **toCorsa;
}OrderPointer;
typedef struct {
    int n;
    Corsa *corse;
    OrderPointer ordinamenti[errore];
    e_key last_update;
    /*Ho n=errore possibili ordinamenti.*/
} tabella_t;

tabella_t leggiLog();
IntervalloId selezionaIntervalloDate (char *d1,char*d2,tabella_t *tab, e_key chiave);
IntervalloId selezionaIntervalloTratta (char *s1,tabella_t *tab, e_key chiave);
e_comandi leggiComando(char c_tabella[][S]);
e_comandi wrapper_ricerca (tabella_t *tab, e_key chiave);
e_comandi wrapper_stampa (tabella_t *tab);
e_comandi  wrapper_ordina (tabella_t *tab,e_key chiave);
e_comandi wrapper_ritardo_tot (tabella_t *tab);
e_comandi wrapper_intervallo_date (tabella_t *tab,int ritardo);
e_comandi wrapper_load_file (tabella_t *tab);
e_comandi chiave2cod (e_key chiave);
e_key str2key(char *str);
void toMinuscolo(char str[]);
void selezionaDati(e_comandi c, tabella_t *tab );
void toMaiuscolo(char str[]);
void stampaTratta(FILE *out,Corsa *c);
void stampaFile (tabella_t *tab,e_key  chiave,char *fileout);
void stampaSchermo (tabella_t *tab, e_key chiave);
void  scambiaCorsaPointer(tabella_t *tab, int indice, e_key chiave);
void scambiaStr (char *item1,char *item2);
void Ordina (tabella_t *tab,e_key chiave);
void wrapper_MergeSort (tabella_t *tab, e_key chiave);
void MergeSort (tabella_t *tab, e_key chiave,Corsa *v[],int inf,int sup);
void Merge (tabella_t *tab,e_key chiave, Corsa *v[], int inf,int m,int sup);
void freeHeap (tabella_t *tab);
int linearSearch (tabella_t *tab, char *s, e_key);
int binarySearch (tabella_t *tab, char *s, int inf, int sup,e_key, e_searchType mod);
int confrontaStr (char *s1,char *s2, int l);
int confrontaTime (Corsa *item1, Corsa *item2 );
int isDate (char *date);
char *chiave2str(e_key chiave, tabella_t *tab, int indice);

/*Il Vettore di struct OrderPointer contiene tutte infomrazioni riguardanti gli ordinamenti per chiave
 * accedo tramite tab->ordinamenri[chiave].toCorsa[indice] al puntatore a corsa. Il vettore dei puntatori su richiesta dell'utente
 * viene ordinato per la chiave a cui corrisponde. Ho scelto di non ordinare all'inizio del programma il vettore per testare le due possibilità di
 * ricerca implementate.In questo modo il programma sceglie quale metodo di ricerca utilizzare in funzione della esistenza
 * o meno di un vettore di puntatori ordinato per una determinata chiave.
 * Ho effettuato due modifiche all'algoritmo "classico" di ricerca binaria per: 1. ottenere l'indice al primo elemento se presenti più occorrenze
 * 2.Funzionare anche in maniera non esatta, ovvero in caso di mancata corrispondeza ritornare l'indice preceduto o seguito dal valore di ricerca (estremo) e quindi
 * generare degli intervalli di id(rif. sul puntatore relativo alla chiave).*/

static int firstLoad  = 1;

int main() {
    tabella_t tab;
    e_comandi CODcomando;
    int finito = 0;
    char c_tabella[r_errore][S]={ "stampa","ordina_time","ordina_tratta","ordina_partenza","ordina_arrivo","partenza","capolinea","ricerca_date","ricerca_tratta","ritardo","ritardo_tot","date","carica_log","fine" };
    tab = leggiLog();
    tab.last_update = codTratta;
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
    char file[S],str_tmp[S];
    tabella_t tab;
    int i,l;
    FILE *fp;
    /*Alla prima chiamata il file acquisito è quello di default definito alla riga 6. Alle successive chiamate è richiesta
     * l'interazione dell'utente per caricare un nuovo file di LOG. Lo stato fisrtLoad è salvato in una variabile globale*/
    if(firstLoad == 1){
        fp = fopen(LOGFILE,"r");
        firstLoad = 0;
        printf("FILE DI DEFAULT (%s) ACQUISITO - stato: firstLoad = %d\n\n",LOGFILE,firstLoad);
    }
    else{
        do {
            printf("Inserire percorso file LOG: ");
            scanf("%s",file);
            fp =  fopen(file,"r");
            if(fp == NULL)
                printf("Inserire percorso file valido\n\n");
        }while (fp == NULL);
    }
    if(fp == NULL)exit(-1);
    if(fscanf(fp,"%d",&tab.n)!=1)
        exit(-1);

    /*Per esercizio alloco a mano tutti i vettori interni alla struttura
     *Normalmente non ne vale la pena dovendo poi fare una strcpy e portare la complessità della funzione ad O(n^2)*/
    tab.corse = (Corsa *) calloc(tab.n,sizeof(Corsa));
    if (tab.corse  == NULL)
        exit(-1);

    for(i=0; i<errore; i++) {
        tab.ordinamenti[i].chiaveOrdinamento = errore;
        tab.ordinamenti[i].toCorsa = (Corsa ** ) calloc(tab.n,sizeof(Corsa*));
        if (tab.ordinamenti[i].toCorsa  == NULL)
            exit(-1);
    }

    for(i=0; i<tab.n; i++){
        tab.corse[i].delay_time = 0;
        /*Documentazione strdup: https://man7.org/linux/man-pages/man3/strdup.3.html*/
        fscanf(fp,"%s",str_tmp);
        tab.corse[i].codTratta = (char *) strdup(str_tmp);
        if (tab.corse[i].codTratta  == NULL)
            exit(-1);


        fscanf(fp,"%s",str_tmp);
        tab.corse[i].start = (char *) strdup(str_tmp);
        if (tab.corse[i].start  == NULL)
            exit(-1);


        fscanf(fp,"%s",str_tmp);
        tab.corse[i].end = (char *) strdup(str_tmp);
        if (tab.corse[i].end  == NULL)
            exit(-1);


        fscanf(fp,"%s",str_tmp);
        tab.corse[i].date = (char *) strdup(str_tmp);
        if (tab.corse[i].date  == NULL)
            exit(-1);


        fscanf(fp,"%s",str_tmp);
        tab.corse[i].start_time = (char *) strdup(str_tmp);
        if (tab.corse[i].start_time  == NULL)
            exit(-1);


        fscanf(fp,"%s",str_tmp);
        tab.corse[i].end_time = (char *) strdup(str_tmp);
        if (tab.corse[i].end_time  == NULL)
            exit(-1);


        fscanf(fp,"%d",&tab.corse[i].delay_time);

        sscanf(tab.corse[i].start_time,"%d:%d:%d",&tab.corse[i].Hp.h,&tab.corse[i].Hp.m,&tab.corse[i].Hp.s);
        sscanf(tab.corse[i].end_time,"%d:%d:%d",&tab.corse[i].Ha.h,&tab.corse[i].Ha.m,&tab.corse[i].Ha.s);
        sscanf(tab.corse[i].date,"%d/%d/%d",&tab.corse[i].Gd.a,&tab.corse[i].Ha.m,&tab.corse[i].Ha.s);
        tab.ordinamenti[time].toCorsa[i] = tab.ordinamenti[codTratta].toCorsa[i]  = tab.ordinamenti[start].toCorsa[i] =tab.ordinamenti[end].toCorsa[i] = &(tab.corse[i]);
        //printf("%s \n",tab.ordinamenti[end].toCorsa[i]->start);
    }

    fclose(fp);
    return tab;
}

e_comandi leggiComando(char c_tabella[][S]){
    int i;
    char cmd[S];
    e_comandi c;

    char c_tabella_Comandi [r_errore][SS]={
            "1. Stampa log: stampa <nome_file/schermo> <chiave_ordinamento> \nATTENZIONE SE STAMPI SU FILE INSERISCI ESTENSIONE - Se non presente vettore ordinato stampa effettuata secondo last_update",
            "2. ordinamento del vettore per data, e a parita di date per ora: ordina_time",
            "3. ordinamento del vettore per codice di tratta: ordina_tratta",
            "4. ordinamento del vettore per stazione di partenza: ordina_partenza",
            "5. ordinamento del vettore per stazione di arrivo: ordina_arrivo",
            "6. ricerca di una tratta per stazione di partenza: partenza <stazione partenza (anche stringa parziale)>",
            "7. ricerca di una tratta per stazione di arrivo: capolinea <stazione arrivo (anche stringa parziale)>",
            "8. ricerca di una tratta per codice tratta: ricerca_tratta <codice tratta (anche stringa parziale)>",
            "9. ricerca di una tratta per data: ricerca_date <data (anche stringa parziale)>",
            "10. elenca corse in ritardo in un intervallo di date: ritardo <data1> <data1>",
            "11. ritardo complessivo di una tratta: ritardo_tot <codice tratta>",
            "12. elenca corse in un intervallo di date: date <data1> <data1>",
            "13. Carica un nuovo log: carica_log",
            "Termina esecuzione: fine"
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
        case r_ricerca_partenza: cod = wrapper_ricerca(tab,start);
            break;
        case r_ricerca_arrivo: cod = wrapper_ricerca(tab,end);
            break;
        case r_ricerca_codTratta: cod = wrapper_ricerca(tab,codTratta);
            break;
        case r_ricerca_date: cod = wrapper_ricerca(tab,time);
            break;
        case r_ordina_time: cod = wrapper_ordina(tab,time);
            break;
        case r_ordina_partenza: cod = wrapper_ordina(tab,start);
            break;
        case r_ordina_arrivo: cod = wrapper_ordina(tab,end);
            break;
        case r_ordina_tratta: cod = wrapper_ordina(tab,codTratta);
            break;
        case r_ritardo: cod = wrapper_intervallo_date (tab,1);
            break;
        case r_ritardo_tot: cod = wrapper_ritardo_tot(tab);
            break;
        case r_date: cod = wrapper_intervallo_date (tab,0);
            break;
        case r_carica_log: cod = wrapper_load_file(tab);
            break;
        default:exit(-1);
    }

    /*Gli ulteriori parametri del comando sono acquisiti in lettura da
    stdin*/

    if(cod != c)printf("Errore\n");

    return;
}

e_comandi wrapper_load_file (tabella_t *tab){
    e_comandi cod = r_carica_log;
    char cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    freeHeap(tab);
    printf("free heap avvenuta con successo\n");
    *tab = leggiLog();
    return cod;
}

e_comandi wrapper_intervallo_date (tabella_t *tab,int ritardo){
    e_comandi cod;
    int i;
    IntervalloId intervallo;
    char data1[S],data2[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s %s",data1,data2)==2 && isDate(data1) && isDate(data2)){
        if(confrontaStr(data1,data2,0)>0)
            scambiaStr(data1,data2);
        switch (ritardo) {
            case 1: cod = r_ritardo;
                break;
            case 0 : cod = r_date;
                break;
            default: cod=r_errore;
        }
        if(tab->ordinamenti[time].chiaveOrdinamento == time) {
            intervallo = selezionaIntervalloDate(data1,data2,tab,time);
            /*Riduco il numero di strncmp effettuate in quanto ricavo l'intervallo utilizzando l'informazione che esiste
             * un vettore ordinato per tale chiave*/
            printf("Intervallo selezionato da BINARY SEARCH id %d a %d\n",intervallo.idInf,intervallo.idSup);
            if (intervallo.idInf >= 0 &&  intervallo.idSup<=tab->n-1)
                for (int i = intervallo.idInf;i<=intervallo.idSup; i++) {
                    if (ritardo) {
                        if (tab->ordinamenti[time].toCorsa[i]->delay_time > 0)
                            stampaTratta(stdout, tab->ordinamenti[time].toCorsa[i]);
                    }
                    else stampaTratta(stdout, tab->ordinamenti[time].toCorsa[i]);
                }
        }
        else
            for (int i = 0; i < tab->n; i++)
                if(confrontaStr(data1,tab->ordinamenti[time].toCorsa[i]->date,0)<=0 && confrontaStr(data2,tab->ordinamenti[time].toCorsa[i]->date,0)>=0) {
                    if (ritardo) {
                        if (tab->ordinamenti[time].toCorsa[i]->delay_time > 0)
                            stampaTratta(stdout, tab->ordinamenti[time].toCorsa[i]);
                    } else stampaTratta(stdout, tab->ordinamenti[time].toCorsa[i]);
                }
    }
    return cod;
}

e_comandi wrapper_ritardo_tot (tabella_t *tab){
    e_comandi cod= r_errore;
    int i,conta=0;
    char codTrattaS[S],cmd[S];
    IntervalloId intervallo;
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",codTrattaS)==1 && strlen(codTrattaS) == SCODTRATTA ){
        cod = r_ritardo_tot;
        /*Se esiste un ordinamento per type codTratta restringo la ricerca*/
        if(tab->ordinamenti[codTratta].chiaveOrdinamento == codTratta) {
            intervallo = selezionaIntervalloTratta(codTrattaS,tab,codTratta);
            printf("Intervallo selezionato da BINARY SEARCH id %d a %d\n",intervallo.idInf,intervallo.idSup);
            if (intervallo.idInf >= 0 && intervallo.idSup<=tab->n-1)
                for (int i = intervallo.idInf; i <=intervallo.idSup ; i++)
                    if(tab->ordinamenti[codTratta].toCorsa[i]->delay_time>0 )
                        conta=conta+tab->ordinamenti[codTratta].toCorsa[i]->delay_time;
        }
        else {
            for (int i = 0; i < tab->n; i++)
                if(tab->ordinamenti[codTratta].toCorsa[i]->delay_time>0 && confrontaStr(codTrattaS,tab->ordinamenti[codTratta].toCorsa[i]->codTratta,0)==0)
                    conta=conta+tab->ordinamenti[codTratta].toCorsa[i]->delay_time;
        }
        toMaiuscolo(codTrattaS);
        if(conta)
            printf("Ritardo totale per %s: %d %s\n",codTrattaS,conta,conta==1?"minuto":"minuti");
    }
    return cod;
}

e_comandi wrapper_stampa (tabella_t *tab){
    e_comandi cod= r_errore;
    e_key chiave;
    char fileout[S],chiaveS[S],cmd[S];
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s %s",fileout,chiaveS)>=1 ){
        toMinuscolo(fileout);
        if (chiaveS == NULL)
            chiave = tab->last_update;
        else {
            toMinuscolo(chiaveS);
            chiave = str2key(chiaveS);
            if (chiave == errore || tab->ordinamenti[chiave].chiaveOrdinamento != chiave) {
                chiave = tab->last_update;
                printf("Ordinamento per chiave non presente in memora. Stampo per last_update \n");
            }
        }
        cod = r_stampa;
        if(strcmp(fileout,SCHERMO) == 0)
            stampaSchermo(tab,chiave);
        else
            stampaFile(tab,chiave,fileout);
    }
    return cod;
}

e_comandi wrapper_ricerca (tabella_t *tab, e_key chiave){
    e_comandi cod;
    int idCorsa,i,stampa=1;
    char search[S],cmd[S],*str;
    gets(cmd); /* Salto al comando successivo eventuali errori di formattazione input derivanti da questo */
    if(sscanf(cmd,"%s",search)==1 ){
        cod = chiave2cod(chiave);
        if(tab->ordinamenti[chiave].chiaveOrdinamento == chiave){
            printf("\nBINARY SEARCH\n");
            if(SHOW_SWAP)stampaSchermo(tab,chiave);
            idCorsa = binarySearch(tab,search,0,tab->n-1,chiave,esatta);
            /*Binary search attraverso una lieve modifica della ricerca binaria ricorsiva
              ritorna l'id al primo elemento corrispondente ai criteri di ricerca (id rif vettore dei puntatori)
            Se non trova corrispondenze ritorna -1 */
            if (idCorsa >= 0) {
                for (i = idCorsa;  stampa && i < tab->n; i++) {
                    str = chiave2str(chiave,tab,i);
                    if (confrontaStr(str, search, strlen(search)) == 0)
                        stampaTratta(stdout, tab->ordinamenti[chiave].toCorsa[i]);
                    else stampa = 0;
                }
            }
            else printf("Nessuna occorrenza riscontrata per %s",search);
        }
        else {
            printf("\nLINEAR SEARCH\n");
            idCorsa = linearSearch(tab, search,chiave);
            if (idCorsa<1) printf("Nessuna occorrenza riscontrata per %s",search);
        }
    }
    return cod;
}

e_comandi  wrapper_ordina (tabella_t *tab,e_key chiave){
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
    tab->last_update = chiave;
    if(tab->ordinamenti[chiave].chiaveOrdinamento == chiave){
        printf("Vettore di puntatori ORDINATO per chiave di ricerca prensente in memoria. SALTO\n");
        return cod;
    }
    /* Ordina(tab,chiave); Modificato alla versione es3lab6*/
    wrapper_MergeSort(tab,chiave);
    tab->ordinamenti[chiave].chiaveOrdinamento = chiave;
    printf("Ordinamento completato\n");
    return cod;
}

int linearSearch (tabella_t *tab, char *s, e_key chiave){
    int i=0,conta=0;
    char *str;
    for(i=0; i<tab->n; i++){
        str = chiave2str(chiave,tab,i);
        if( confrontaStr(str, s, strlen(s)) ==0) {
            stampaTratta(stdout, tab->ordinamenti[chiave].toCorsa[i]);
            conta = conta+1;
        }
    }
    return conta;
}

int binarySearch (tabella_t *tab, char *s, int inf, int sup,e_key chiave, e_searchType mod) {
    char *str;
    int res;
    if(sup-inf == 0) {
        str =  chiave2str(chiave,tab,inf);
        res = confrontaStr (str, s, strlen(s) );
        if ( res == 0 && mod != segue )
            return inf;
        if( res != 0 && mod == esatta)
            return -1;
        if(res <0 && mod == precede)
            return inf;
        if(res <0 && mod == segue )
            return tab->n-1;
        else return inf-1;

        /*La modifica apportata alla condizione di terminazione estende alla possibilità di corrispondenze
         * non esatte, utile la selezione di intervalli. Per selezionare gli estremi superiori degli intervalli
         * faccio girare la funzione al senso contrario*/

    }
    int m = (sup+inf)/2;
    str =  chiave2str(chiave,tab,m);

    if(mod != precede) {
        if (confrontaStr(str, s, strlen(s)) >= 0)
            return binarySearch(tab, s, inf, m, chiave, mod);
        else
            return binarySearch(tab, s, m + 1, sup, chiave, mod);
    }
    else {
        if (confrontaStr(str, s, strlen(s)) > 0)
            return binarySearch(tab, s, inf, m, chiave, mod);
        else
            return binarySearch(tab, s, m + 1, sup, chiave, mod);
    }

}

void Ordina (tabella_t *tab,e_key chiave){
    /*Bubble sort Algoritmo stabile */
    int i,j,res;
    for(i=0; i<tab->n; i++){
        for(j=0;j<tab->n-i-1; j++){
            switch (chiave) {
                case time:
                    res = confrontaTime(tab->ordinamenti[chiave].toCorsa[j], tab->ordinamenti[chiave].toCorsa[j + 1]);
                    break;
                case codTratta :
                    res = confrontaStr(tab->ordinamenti[chiave].toCorsa[j]->codTratta, tab->ordinamenti[chiave].toCorsa[j + 1]->codTratta, 0);
                    break;
                case start :
                    res = confrontaStr(tab->ordinamenti[chiave].toCorsa[j]->start, tab->ordinamenti[chiave].toCorsa[j + 1]->start, 0);
                    break;
                case end :
                    res = confrontaStr(tab->ordinamenti[chiave].toCorsa[j]->end, tab->ordinamenti[chiave].toCorsa[j + 1]->end, 0);
                    break;
                default:
                    res = -1;
            }
            if(res>0){
                if(SHOW_SWAP){
                    stampaTratta(stdout,tab->ordinamenti[chiave].toCorsa[j]);
                    stampaTratta(stdout,tab->ordinamenti[chiave].toCorsa[j+1]);
                    printf("------------SCAMBIA-------------");
                }
                scambiaCorsaPointer(tab,j,chiave);
            }

        }
    }

    return;
}

void wrapper_MergeSort (tabella_t *tab, e_key chiave){
    Corsa **v = (Corsa **) malloc(tab->n*sizeof(Corsa*));
    int inf=0,sup=tab->n-1;
    MergeSort (tab,chiave,v,inf,sup);
    free(v);
    return;
}

void MergeSort (tabella_t *tab, e_key chiave,Corsa *v[],int inf,int sup){
    int q = (sup+inf)/2;
    if(sup<=inf)
        return;
    MergeSort(tab,chiave,v,inf,q);
    MergeSort(tab,chiave,v,q+1,sup);
    Merge (tab,chiave,v,inf,q,sup);
}

void Merge (tabella_t *tab,e_key chiave, Corsa *v[], int inf,int m,int sup){
    int i,j,k,res;
    i = inf;
    j = m+1;
    for ( k = inf; k <=sup ; k++) {
        if (i>m)
            v[k] = tab->ordinamenti[chiave].toCorsa[j++];
        else if (j>sup)
            v[k] = tab->ordinamenti[chiave].toCorsa[i++];
        else {
            switch (chiave) {
                case time:
                    res = confrontaTime(tab->ordinamenti[chiave].toCorsa[i], tab->ordinamenti[chiave].toCorsa[j]);
                    break;
                case codTratta :
                    res = confrontaStr(tab->ordinamenti[chiave].toCorsa[i]->codTratta, tab->ordinamenti[chiave].toCorsa[j]->codTratta, 0);
                    break;
                case start :
                    res = confrontaStr(tab->ordinamenti[chiave].toCorsa[i]->start, tab->ordinamenti[chiave].toCorsa[j]->start, 0);
                    break;
                case end :
                    res = confrontaStr(tab->ordinamenti[chiave].toCorsa[i]->end, tab->ordinamenti[chiave].toCorsa[j]->end, 0);
                    break;
                default:
                    res = -1;
            }
            if (res > 0) /* item i > item j */
                v[k] = tab->ordinamenti[chiave].toCorsa[j++];
            else /* item i <= item j */
                v[k] = tab->ordinamenti[chiave].toCorsa[i++];
        }
    }

    for ( k=inf; k<= sup; k++)
        tab->ordinamenti[chiave].toCorsa[k] = v[k];
}
int confrontaTime (Corsa *item1, Corsa *item2 ){
    int res = confrontaStr(item1->date,item2->date,0);
    if(res == 0){
        res = confrontaStr(item1->start_time,item2->start_time,0);
        if(res == 0)
            res = confrontaStr(item1->end_time,item2->end_time,0);
    }
    return  res;
}

void  scambiaCorsaPointer(tabella_t *tab, int indice, e_key chiave){
    Corsa *tmp = tab->ordinamenti[chiave].toCorsa[indice];
    tab->ordinamenti[chiave].toCorsa[indice] = tab->ordinamenti[chiave].toCorsa[indice+1];
    tab->ordinamenti[chiave].toCorsa[indice+1] = tmp;
    return;
}

void scambiaStr (char *item1,char *item2){
    char *tmp = item1;
    item1 = item2;
    item2 = tmp;
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
    char *item1,*item2;
    int res ;
    item1 = strdup(s1);
    item2 = strdup(s2);
    toMaiuscolo(item1);
    toMaiuscolo(item2);
    if(l !=0 )
        res = strncmp(item1,item2,l);
    else
        res = strcmp(item1,item2);
    free(item2);
    free(item1);
    return res;
}

void stampaSchermo (tabella_t *tab, e_key chiave){
    int i=0;
    for(i=0; i<tab->n; i++)
        stampaTratta(stdout,tab->ordinamenti[chiave].toCorsa[i]);
    return;
}

void stampaFile (tabella_t *tab,e_key  chiave,char *fileout){
    FILE *out = fopen(fileout,"w");
    int i;
    for(i=0; i<tab->n; i++)
        stampaTratta(out,tab->ordinamenti[chiave].toCorsa[i]);
    fclose(out);
    return;
}

void stampaTratta(FILE *out,Corsa *c){
    /*Ricevuti in input puntatore a tratta e file di output stampa*/
    printf("\n");
    fprintf(out,"%-7s da %-13s a %-15s del %-10s PARTENZA: %-10s ARRIVO: %-10s RITARDO: %d %s\n\n",c->codTratta,c->start,
            c->end,c->date,c->start_time,c->end_time,c->delay_time,c->delay_time==1?"minuto":"minuti");
    return;
}

int isDate (char *date){
    /*formato data: YYYY/MM/DD
    Ricodifico date se espresse nel formato DD/MM/YYYY
     Date non vettori dinamici, rimangono arginati in questo stack*/
    char y[S],m[S],d[S];
    int i;
    for(i=0; i<strlen(date); i++)
        if(date[i]=='/')
            date[i]=' ';
    if(sscanf(date,"%s %s %s",y,m,d)==3){
        if(strlen(y)==4 && strlen(m)==2 && strlen(d)==2){
            for(i=0; i<strlen(date); i++)
                if (date[i] == ' ')
                    date[i] = '/';
            return 1;
        }
        else if (strlen(y)==2 && strlen(m)==2 && strlen(d)==4){
            sprintf(date,"%s/%s/%s",d,m,y);
            return 1;
        }
    }
    return 0;
}

e_comandi chiave2cod (e_key chiave){
    e_comandi cod;
    switch (chiave) {
        case time: cod = r_ricerca_date;
            break;
        case codTratta : cod = r_ricerca_codTratta;
            break;
        case start : cod = r_ricerca_partenza;
            break;
        case end : cod = r_ricerca_arrivo;
            break;
        default: cod=r_errore;
    }
    return cod;
}

char *chiave2str(e_key chiave, tabella_t *tab, int indice){
    char *str;
    switch (chiave) {
        case time: str = tab->ordinamenti[chiave].toCorsa[indice]->date;
            break;
        case codTratta : str = tab->ordinamenti[chiave].toCorsa[indice]->codTratta;
            break;
        case start : str = tab->ordinamenti[chiave].toCorsa[indice]->start;
            break;
        case end : str = tab->ordinamenti[chiave].toCorsa[indice]->end;
            break;
    }
    return str;
}
IntervalloId selezionaIntervalloDate (char *d1,char*d2,tabella_t *tab, e_key chiave){
    IntervalloId intervallo;
    /*L'id è effettivamente la prima "corsa" (puntatore ecc...) sulla quale far partire la stampa
     * passo idInf alla chiamata per calcolare idSup, in quanto sarà inutile cercare più in basso*/
    intervallo.idInf = binarySearch(tab, d1, 0, tab->n - 1, chiave, segue);
    if(intervallo.idInf+1<tab->n)
        intervallo.idSup = binarySearch(tab, d2, intervallo.idInf+1, tab->n-1, chiave, precede);
    else
        intervallo.idSup = -1;
    if(intervallo.idSup>=0  )intervallo.idInf +=1;/*Qualora l'id superiore trova occorrenze sblocco l'id inferiore.*/
    return intervallo;
}
IntervalloId selezionaIntervalloTratta (char *s1,tabella_t *tab, e_key chiave){
    IntervalloId intervallo;
    intervallo.idInf = binarySearch(tab, s1, 0, tab->n - 1, chiave, esatta);
    intervallo.idSup = binarySearch(tab, s1, intervallo.idInf, tab->n - 1, chiave, precede);
    if(intervallo.idInf>intervallo.idSup) intervallo.idSup = intervallo.idInf;
    return intervallo;
}
e_key str2key(char *str){
    char chiavi [errore][S] ={"codtratta","start","end","time"};
    e_key chiave = codTratta;
    while (chiave<errore && strcmp(str,chiavi[chiave]) != 0)
        chiave ++;
    return chiave;
}

void freeHeap (tabella_t *tab){
    int i;
    /*Dealloco secondo ordine inverso*/
    printf("CORSE: %d ",tab->n);

    for(i=0; i<tab->n; i++) {
        printf("DEALLOCATE: %s  ",tab->corse[i].codTratta);
        free(tab->corse[i].codTratta);
        free(tab->corse[i].start);
        free(tab->corse[i].end);
        free(tab->corse[i].date);
        free(tab->corse[i].start_time);
        free(tab->corse[i].end_time);

    }
    free (tab->corse);
    for(i=0; i<errore; i++)
        free (tab->ordinamenti[i].toCorsa);

    return;
}