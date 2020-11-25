#include <stdio.h>
#include <stdlib.h>

#define S 50+1
#define SHOW_INFO 0

typedef enum tipo{
    e_zaffiro,e_rubino,e_topazio,e_smeraldo,e_void
} e_tipo;
typedef struct{
    e_tipo type;
    int num,occ_now,val,rip_now;
}Item;
typedef struct{
    e_tipo *sol;
    int n,value;
}Sol;
typedef struct{
    Item *pietre;
    Sol p_sol,best_sol;
    int n_max,max_rip,max_value;
}tab;

tab init_set (FILE *);
void wrapper_dispRip (tab *t);
void free_tab(tab *DB);
void dispRip (tab *t, int pos);
void update_best(tab *t,int n);
void stampa(tab *t);
int pruning (tab *t, int pos, int i);
int checkSol (tab *t,int n);

int main() {
    setbuf(stdout, 0);
    char str[S];
    FILE *fp;
    int i,test_num;
    do{
        printf("Inserire percorso file: ");
        gets(str);
        fp = fopen(str,"r");
        if(fp == NULL)
            printf("Percorso file non valido!\n");
    }while (fp == NULL);
    fscanf(fp,"%d",&test_num);

    for (int i = 0; i < test_num; i++) {
        printf("\n\nTEST #%d\n",i+1);
        tab DB = init_set(fp);
        wrapper_dispRip (&DB);
        free_tab(&DB);
    }

    fclose(fp);
    return 0;
}

tab init_set (FILE *fp){
    tab tab_t;
    int j;
    tab_t.pietre = malloc(e_void*sizeof(Item));
    tab_t.n_max = tab_t.best_sol.n = tab_t.best_sol.value = 0;
    tab_t.best_sol.sol = NULL;
    for (int j = 0; j < e_void; j++) {
        fscanf(fp,"%d",&tab_t.pietre[j].num);
        tab_t.n_max += tab_t.pietre[j].num;
        tab_t.pietre[j].type = j;
        tab_t.pietre[j].occ_now = 0;
        tab_t.pietre[j].rip_now = 0; /*Attuali ripetizioni consecutive della gemma*/
    }
    for (int j = 0; j < e_void; j++){
        fscanf(fp,"%d",&tab_t.pietre[j].val);
        tab_t.max_value += tab_t.pietre[j].val*tab_t.pietre[j].num;
    }
    fscanf(fp,"%d",&tab_t.max_rip);
    return tab_t;
}

void wrapper_dispRip (tab *t){
    t->p_sol.n = t->n_max;
    t->p_sol.sol = calloc(t->p_sol.n,sizeof(e_void));
    t->p_sol.value  =0;
    dispRip(t,0);
    free(t->p_sol.sol);
    stampa(t);
}

void dispRip (tab *t, int pos){
    int i;
    if(pos >= t->p_sol.n){
        if(t->p_sol.value > t->best_sol.value)
            update_best(t,t->p_sol.n);
        return;
    }

    for(i=0; i<e_void; i++){
        if(t->pietre[i].occ_now < t->pietre[i].num && (pos==0 || !pruning(t,pos,i) ) ) {
            t->pietre[i].occ_now++;
            if(pos == 0 || t->p_sol.sol[pos-1]==t->pietre[i].type)
                t->pietre[i].rip_now++;
            t->p_sol.sol[pos] = t->pietre[i].type;
            t->p_sol.value += t->pietre[i].val;
            //printf("%d\n",t->p_sol.value);
            dispRip(t, pos + 1);
            t->p_sol.value -= t->pietre[i].val;
            if(t->pietre[i].rip_now > 0)
                t->pietre[i].rip_now --; /*Cambio pietra*/
            t->pietre[i].occ_now--;
        }else{
            if(t->p_sol.value > t->best_sol.value)
                update_best(t,pos); /*Con pos cardinalilà soluzione*/
                //stampa(t);
        }
    }
}
void update_best(tab *t,int n ){
    if(checkSol(t,n)==0) {
        t->best_sol.value = t->p_sol.value;
        t->best_sol.n = n;
        if (t->best_sol.sol != NULL)
            free(t->best_sol.sol);
        t->best_sol.sol = malloc(t->best_sol.n * sizeof(e_tipo));
        for (int i = 0; i < t->best_sol.n; i++)
            t->best_sol.sol[i] = t->p_sol.sol[i];
    }

}
int pruning (tab *t, int pos, int i){
/*i indice esaminato*/
    e_tipo prev = t->p_sol.sol[pos-1];

    if(t->pietre[i].type == e_zaffiro && (t->pietre[e_zaffiro].occ_now+1  > t->pietre[e_smeraldo].occ_now))
        return 1;

    if(t->pietre[i].type == prev && t->pietre[i].rip_now+1>t->max_rip)
        return 1;

    if((prev == e_zaffiro) && (t->pietre[i].type != e_zaffiro) && (t->pietre[i].type != e_rubino))
        return 1;
    if(prev == e_smeraldo && (t->pietre[i].type != e_smeraldo) && (t->pietre[i].type != e_topazio))
        return 1;
    if(prev == e_rubino &&(t->pietre[i].type != e_smeraldo) && (t->pietre[i].type != e_topazio))
        return 1;
    if(prev == e_topazio && (t->pietre[i].type != e_zaffiro) && (t->pietre[i].type != e_rubino))
        return 1;
    return 0;
}

int checkSol (tab *t,int n){
    int i = 1,check=1;
    e_tipo test = t->p_sol.sol[0];
    while (i<n){
        if(t->p_sol.sol[i]==test)
            check++;
        else{
            test = t->p_sol.sol[i];
            check = 1;
        }
        if(check>t->max_rip)
            return 1;
        i++;
    }
    return n-i;
}
void stampa(tab *t){
    int i;
    char dict[e_void] = {'z','r','t','s'};
    for(i=0; i<t->best_sol.n; i++)
        printf("%c",dict[t->best_sol.sol[i]]);
    printf("\nZAFFIRO[%d] %d, RUBINO[%d] %d, TOPAZIO[%d]  %d, SMERALDO[%d]  %d, TOT:%d\nLUNGHEZZA CATENA: %d VALORE CATENA: %d\nMAX_RIP: %d\n",t->pietre[e_zaffiro].val,t->pietre[e_zaffiro].num,
           t->pietre[e_rubino].val,t->pietre[e_rubino].num,t->pietre[e_topazio].val, t->pietre[e_topazio].num,t->pietre[e_smeraldo].val, t->pietre[e_smeraldo].num,t->n_max,t->best_sol.n,t->best_sol.value,t->max_rip);
}

void free_tab(tab *DB){
    free(DB->pietre);
    free(DB->best_sol.sol);
}

