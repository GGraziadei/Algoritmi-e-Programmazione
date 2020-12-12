#include <stdio.h>
#include <stdlib.h>

#define S 50+1
#define SHOW_INFO 0

typedef enum tipo{
    e_zaffiro,e_rubino,e_topazio,e_smeraldo,e_void
} e_tipo;
typedef struct{
    e_tipo type;
    int num,val;
}Item;

typedef struct{
    Item *pietre;
    int n_max,max_value,*maxKnown;
}tab;

tab init_set (FILE *);
void free_tab(tab *DB);
int openFile (FILE **fp);
int MaxValueDP(e_tipo pietra_prev,tab *t, int pos);
int check (e_tipo p1, e_tipo prev);
int fZ (tab *t);

int main() {
    setbuf(stdout, 0);
    FILE *fp;
    int i,test_num;
    test_num = openFile(&fp);
    for (int i = 0; i < test_num; i++) {
        printf("\n\nTEST #%d\n",i+1);
        tab DB = init_set(fp);
        printf("%d   \n",fZ(&DB));
        free_tab(&DB);
    }

    fclose(fp);
    return 0;
}

int fZ (tab *t){
    int max = 0;
    t->maxKnown = (int *) calloc(t->n_max,sizeof (int)); /*Ogni pietra ha valore >0 questo implioca che è possibile
    * inizializzare a 0 maxKnown*/
    t->maxKnown[0] = t->pietre[e_zaffiro].val;
    max = MaxValueDP(e_zaffiro,t,1);
    free(t->maxKnown);
    return max;
}
int MaxValueDP(e_tipo pietra_prev,tab *t, int pos){
    int i,tmp,max=0;
    if(t->maxKnown[pos]>0)
        return t->maxKnown[pos];
    else
        for(i=0; i<e_void; i++)
            if(check(t->pietre[i].type,pietra_prev) == 0)
                if((tmp = MaxValueDP(t->pietre[i].type,t,pos+1)) > max)
                    max = tmp;
    t->maxKnown[pos] = max;
    return max;
}
tab init_set (FILE *fp){
    tab tab_t;
    int j;
    tab_t.pietre = malloc(e_void*sizeof(Item));
    tab_t.n_max = 0;
    for (int j = 0; j < e_void; j++) {
        fscanf(fp,"%d",&tab_t.pietre[j].num);
        tab_t.n_max += tab_t.pietre[j].num;
        tab_t.pietre[j].type = j;
    }
    for (int j = 0; j < e_void; j++){
        fscanf(fp,"%d",&tab_t.pietre[j].val);
        tab_t.max_value += tab_t.pietre[j].val*tab_t.pietre[j].num;
    }
    return tab_t;
}

int check (e_tipo p1, e_tipo prev){
    if((prev == e_zaffiro) && (p1 != e_zaffiro) && (p1 != e_rubino))
        return 1;
    if(prev == e_smeraldo && (p1 != e_smeraldo) && (p1 != e_topazio))
        return 1;
    if(prev == e_rubino &&(p1 != e_smeraldo) && (p1 != e_topazio))
        return 1;
    if(prev == e_topazio && (p1 != e_zaffiro) && (p1 != e_rubino))
        return 1;
    return 0;
}



void free_tab(tab *DB){
    free(DB->pietre);
}

int openFile (FILE **fp){
    int i;
    char str[S];
    do{
        printf("Inserire percorso file: ");
        gets(str);
        (*fp) = fopen(str,"r");
        if((*fp) == NULL)
            printf("Percorso file non valido!\n");
    }while ((*fp) == NULL);
    fscanf((*fp),"%d",&i);
    return i;
}