#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define S 50+1
#define NUM_CAMPI 7
#define TODAY_Y 2020

typedef enum {
    r_add_file, r_add_keyboard, r_search_code, r_delete_code, r_delete_date,r_print_file,r_fine,r_errore
}e_comandi;
typedef enum {
    s_schermo,s_file
}e_add_mode;
typedef struct {
     int a;
     int m;
     int g;
} data_t;
typedef struct {
    unsigned int cap;
    char *city;
    char *via;
} indirizzo_t;
typedef struct {
    char *id;
    char *nome;
    char *cognome;
    data_t b_day;
    indirizzo_t address;
}Item;
typedef struct node_t *link;
struct node_t{
    Item val;
    link next;
};
link new_node (Item val,link next);
link sort_ins (link h, Item user_x);
link selezionaDati(e_comandi c, link head );
link wrapper_add_item (e_add_mode ,link head);
link linearSearch (link h,char *value);
link wrapper_extract (link head, char *code);
link  wrapper_extract_code (link head);
link  wrapper_extract_date (link head);
data_t str2date (char *str);
e_comandi leggiComando();
Item item_gen_str (char *);
Item item_gen_cmd ();
Item listExtractKeyP (link *hp,char* value);
Item ItemSetVOID ();
data_t dataSetVOID();
indirizzo_t indirizzoSetVOID();
int confrontaDate (data_t d1, data_t d2);
void stampaItem(FILE *out,Item i);
void stampa (link h,FILE *out);
void stampaRev (link h,FILE *out);
void wrapper_stampa (link head);
void wrapper_search_code (link head);
void wrapper_stampaSchermo (int mod, link head);
void wrapper_stampaFile (int mod, char *fileout, link head);
void toMaiuscolo(char *str);
void toMinuscolo(char *str);
void free_node (link x);
void free_item (Item x);
void malloc2dR_str (char *(*p),char *a);



int main() {
    int fine = 0;
    link head = NULL; /*Lista vuota*/
    e_comandi CODcomando;
    while(fine == 0){
        CODcomando = leggiComando();
        if(CODcomando != r_fine)
            head = selezionaDati (CODcomando,head);
        else
            fine =1;
    }
    return 0;
}

e_comandi leggiComando(){
    int i;
    char cmd[S];
    e_comandi c;
    char *c_tabella[r_errore]={  "add_file","add_keyboard","search_code",
                                 "delete_code", "delete_date","print_file","fine" };
    char *c_tabella_Comandi[r_errore]={
            "1. acquisizione ed inserimento ordinato di un nuovo elemento in lista da tastiera: add_keyboard",
            "2. acquisizione ed inserimento ordinato di nuovi elementi in lista da file: add_file",
            "3. ricerca, per codice, di un elemento: search_code <codice>",
            "4. cancellazione di un elemento dalla lista, previa ricerca per codice: delete_code <code>",
            "5. cancellazione di tutti gli elementi con date comprese tra le 2 date lette: delete_date <data1> <data2>",
            "6. stampa lista su file: print_file <nome_file.estensione>",
            "Termina esecuzione: fine"
    };

    do{
        fflush(stdin);
        printf("---------- MENU DI SELEZIONE ----------\n");
        for(i=0; i<r_errore; i++)
            printf("%s \n",c_tabella_Comandi[i] );
        printf("Inserire comando come da specifiche sopra indicate: ");

        if(scanf("%s",cmd)==1){
            toMinuscolo(cmd);
            c = r_add_file;
            while(c<r_errore && strcmp(c_tabella[c],cmd)!=0 )
                c++;
        }
        if(c==r_errore)
            printf("Comando non valido\n\n");
    }while(c == r_errore);
    //printf("Comando selezionato: %s",c_tabella[c]);
    return c;
}

link selezionaDati(e_comandi c, link head ){
/*Comandi disponibili
 r_add_file, r_add_keyboard, r_search_code, r_delete_code, r_delete_date,r_print_file,r_fine,r_errore*/
    e_comandi cod;

    switch (c) {
        case r_add_keyboard: head = wrapper_add_item(s_schermo,head);
            break;
        case r_add_file: head = wrapper_add_item(s_file,head);
            break;
        case r_print_file: wrapper_stampa(head);
            break;
        case r_search_code: wrapper_search_code(head);
            break;
        case r_delete_code: head = wrapper_extract_code(head);
            break;
        case r_delete_date: head = wrapper_extract_date(head);
            break;
        default:exit(-1);
    }

    /*Gli ulteriori parametri del comando sono acquisiti in lettura da
    stdin*/

    return head;
}

/*SEZIONE WRAPPER PER COMANDO */

link wrapper_add_item (e_add_mode mode,link head){
    printf("Import Anagrafica\n");
    FILE *fp;
    char file[S],tmp_str[NUM_CAMPI*S];
    Item user_x;
    if (mode == s_schermo){
        user_x = item_gen_cmd();
        head =  sort_ins(head,user_x);
    }
    else if (mode == s_file){
        do {
            printf("Inserire percorso file ANAGRAFICA: ");
            scanf("%s",file);
            fp =  fopen(file,"r");
            if(fp == NULL)
                printf("Inserire percorso file valido\n\n");
        }while (fp == NULL);
        while (fgets(tmp_str,NUM_CAMPI*S,fp) != NULL) {
            user_x = item_gen_str(tmp_str);
            head =  sort_ins(head,user_x);
        }
        fclose(fp);
    }
    wrapper_stampaSchermo(0,head);
    return head;
}

void wrapper_stampa (link head){
     char file[S];
     int mod;
     fscanf(stdin,"%s",file);
     if (head == NULL){
         printf("Lista VUOTA !\n\n");
         return;
     }
     printf("Seleziona modalita di stampa NORMAL(0) - REVERSE(1): ");
     scanf("%d",&mod);
     wrapper_stampaFile(mod,file,head);
     wrapper_stampaSchermo(mod,head); /*SOLO PER DEBUG*/
}

void wrapper_search_code (link head){
    link x;
    char code[S];
    if(fscanf(stdin,"%s",code)==1) {
        if (code[0] > 'A')
            toMaiuscolo(code);
        x = linearSearch(head,code);
        if (x == NULL)
            printf("Nessuna corrispondenza trovata per %s!\n",code);
        else
            stampaItem(stdout, x->val);
    }
}

link  wrapper_extract_code (link head){
    char code[S];
    Item  i;
    if(fscanf(stdin,"%s",code)==1) {
        if (code[0] > 'A')
            toMaiuscolo(code);
        head = wrapper_extract(head,code);
    }
    return head ;
}

link  wrapper_extract_date (link head){
    Item i;
    char data1[S],data2[S];
    data_t d1,d2,tmp;
    while (fscanf(stdin,"%s %s",data1,data2)!=2)
        printf("Inserire intervallo date: ");
    d2 = str2date(data2);
    d1 = str2date(data1);
    if (confrontaDate(d1,d2)>0){
        printf("Errore logica intervallo date, nuovo intervallo [%s ; %s]");
        tmp = d1;
        d1 = d2;
        d2 = tmp;
    }
    while (head != NULL && confrontaDate(d2,head->val.b_day)>=0 && confrontaDate(d1,head->val.b_day)<=0)
        head = wrapper_extract (head, head->val.id);
    return head ;
}

/*ESTRAZIONE DA CODICE*/
link wrapper_extract (link head, char *code){
    Item i;
    i = listExtractKeyP (&head,code);
    if(i.id != NULL){
        printf("DELETE: ");
        stampaItem(stdout,i);
        free_item(i);
    }
    else
        printf("Nessuna corrispondenza trovata per %s!\n",code);
    return head;
}

Item listExtractKeyP (link *hp,char* value){
    link *xp,t;
    Item i = ItemSetVOID();
    int res;
    for(xp=hp; (*xp)!=NULL; xp=&((*xp)->next)){
        t = *xp;
        res = strcmp(t->val.id,value);
        if( res == 0 ){
            *xp = t->next;
            i = t->val;
            free_node(t);
            return i;
        }
    }
    return i;
}

/*AGGIUNTA NODO LISTA ORDINATA*/
link new_node (Item val,link next){
    link x = malloc(sizeof(*x));
    if (x == NULL)
        exit(-1);
    x->val = val;
    x->next = next;
    return x;
}
link sort_ins (link h, Item user_x){
    link x,p;
    data_t key = user_x.b_day;
    if (h == NULL || confrontaDate(key,h->val.b_day) > 0)
        return new_node(user_x,h);
    for (x = h->next, p=h; x!=NULL && confrontaDate(key,x->val.b_day)<0 ;p=x,x=p->next);
    p->next = new_node(user_x,x);
    return h;
}

/*RICERCA LINEARE DATO CODICE*/
link linearSearch (link h,char *value){
    link i;
    int res;
    for(i=h; i!=NULL; i=i->next){
        res =  strcmp(i->val.id,value);
        if(res == 0)
            return i;
    }
    return NULL;
}

/*STAMPA FILE/SCHERMO SINGOLO_ITEM/INTERA LISTA ORDINE_NORMAL/ORDINE_REVERSE */
void wrapper_stampaFile (int mod, char *fileout, link head){
    FILE *out = fopen(fileout,"w");
    if(mod == 0)
        stampa(head,out);
    else
        stampaRev(head,out);
    fclose(out);
}
void wrapper_stampaSchermo (int mod, link head){
    if(mod == 0)
        stampa(head,stdout);
    else
        stampaRev(head,stdout);
    printf("\n\n");
}
void stampa (link h,FILE *out){
    if(h == NULL)
        return;
    stampaItem(out,h->val);
    stampa(h->next,out);
}
void stampaRev (link h,FILE *out){
    if(h == NULL)
        return;
    stampaRev(h->next,out);
    stampaItem(out,h->val);
}
void stampaItem(FILE *out,Item i){
    /*Ricevuti in input Item e file di output stampa*/
    printf("\n");
    fprintf(out,"%s %s %s %02d/%02d/%04d %s %s %05d\n",i.id,i.nome,i.cognome,i.b_day.g,i.b_day.m,i.b_day.a,i.address.via,
            i.address.city,i.address.cap);
    return;
}

/*GESTIONE TIPO DATA*/
data_t str2date (char *str){
    data_t test;
    if(sscanf(str,"%d/%d/%d",&test.g,&test.m,&test.a) == 3 && test.g<=31 && test.m<=12 && test.a<=TODAY_Y )
        return test;
    else {
        printf("Data %s NON VALIDA ! Inserire data valida: ",str);
        scanf("%s",str);
        return str2date(str); /*Ricorsione fino all'ottenimento di una data valida*/
    }
}

int confrontaDate (data_t d1, data_t d2) {
    /* 0=>pari data, >0 d1 successivo a d2, <0 d2 successivo a d1*/
    if (d1.a != d2.a)
        return (d1.a-d2.a);
    else if (d1.m != d2.m)
        return (d1.m-d2.m);
    else if (d1.g != d2.g)
        return (d1.g-d2.g);
    else return 0;
}

/*VARIE */
void toMinuscolo (char *str){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]<'a')
            str[i] = tolower(str[i]);
    return;
}
void toMaiuscolo(char *str){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]>='a')
            str[i] = toupper(str[i]);
    return;
}

/*GESTIONE MEMORIA*/
Item item_gen_str(char *str){
    /*Il testo dell'esercizio precisa che le stringhe non contengono spazi, le acquisisco con scanf()*/
    char tmp_str[S];
    Item i;
    int j;
    sscanf(str,"%s%n",tmp_str,&j);
    malloc2dR_str(&(i.id),tmp_str);

    str = &str[j];
    sscanf(str,"%s%n",tmp_str,&j);
    malloc2dR_str(&(i.nome),tmp_str);

    str = &str[j];
    sscanf(str,"%s%n",tmp_str,&j);
    malloc2dR_str(&(i.cognome),tmp_str);

    str = &str[j];
    sscanf(str,"%s%n",tmp_str,&j);
    i.b_day = str2date(tmp_str);

    str = &str[j];
    sscanf(str,"%s%n",tmp_str,&j);
    malloc2dR_str(&(i.address.city),tmp_str);

    str = &str[j];
    sscanf(str,"%s%n",tmp_str,&j);
    malloc2dR_str(&(i.address.via),tmp_str);

    str = &str[j];
    sscanf(str,"%s%n",tmp_str,&j);
    sscanf(tmp_str,"%d",&i.address.cap);

    return i;
}
Item item_gen_cmd(){
    /*Il testo dell'esercizio precisa che le stringhe non contengono spazi, le acquisisco con scanf()*/
    char tmp_str[S];
    int l=0;
    Item i;
    do{
        fflush(stdin);
        printf("Inserire CODICE (AXXXX): ");
        scanf("%s",tmp_str);
        l = strlen(tmp_str);
        if (tmp_str[0]>'A')
            toMaiuscolo(tmp_str);
    }while(l!=5 || tmp_str[0]!='A');
    malloc2dR_str(&(i.id),tmp_str);
    //printf("ID %04d",i.id);
    printf("Inserire NOME: ");
    scanf("%s",tmp_str);
    malloc2dR_str(&(i.nome),tmp_str);


    printf("Inserire COGNOME: ");
    scanf("%s",tmp_str);
    malloc2dR_str(&(i.cognome),tmp_str);

    printf("Inserire DATA DI NASCITA (GG/MM/AAAA): ");
    scanf("%s",tmp_str);
    i.b_day = str2date(tmp_str);

    printf("Inserire VIA: ");
    scanf("%s",tmp_str);
    malloc2dR_str(&(i.address.via),tmp_str);

    printf("Inserire CITTA: ");
    scanf("%s",tmp_str);
    malloc2dR_str(&(i.address.city),tmp_str);

    printf("Inserire CAP: ");
    scanf("%d",&i.address.cap);

    return i;
}

Item ItemSetVOID (){
    Item i;
    i.id = i.nome = i.cognome = NULL;
    i.b_day = dataSetVOID();
    i.address = indirizzoSetVOID();
    return i;
}

void free_node (link x){
    free(x);
}

data_t dataSetVOID(){
    data_t i;
    i.a = i.g = i.m = -1;
    return i;
}
indirizzo_t indirizzoSetVOID(){
    indirizzo_t i;
    i.city=i.via = NULL;
    i.cap = -1;
    return i;
}

void free_item (Item x){
    free(x.id);
    free(x.nome);
    free(x.cognome);
    free(x.address.city);
    free(x.address.via);
}
void malloc2dR_str (char *(*p),char *a){
    (*p) =  strdup(a);
    if ((*p) == NULL)
        exit(-1);
}
