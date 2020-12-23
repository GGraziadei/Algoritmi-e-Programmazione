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
}tab_t;

tab_t init_set (FILE *);
void wrapper_dispRip (tab_t *t);
void free_tab(tab_t *DB);
void dispRip (tab_t *t, int pos);
void update_best(tab_t *t,int n);
void stampa(tab_t *t);
int pruning (tab_t *t, int pos, int i);
int checkSol (Sol p_sol,int max_rip,int n);

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
        tab_t DB = init_set(fp);
        wrapper_dispRip (&DB);
        free_tab(&DB);
    }

    fclose(fp);
    return 0;
}

tab_t init_set (FILE *fp){
    tab_t tab_t;
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

void wrapper_dispRip (tab_t *t){
    int i;
    t->p_sol.n = t->n_max;
    t->p_sol.sol = malloc(t->p_sol.n*sizeof(e_tipo));
    t->p_sol.value  =0;
    for(i=0; i<t->p_sol.n; i++)
        t->p_sol.sol[i] = e_void;
    dispRip(t,0);
    free(t->p_sol.sol);
    stampa(t);
}

void dispRip (tab_t *t, int pos){
    int i;
    if(pos >= t->p_sol.n){
        if((t->p_sol.value > t->best_sol.value)
            &&(t->pietre[e_zaffiro].occ_now <= t->pietre[e_smeraldo].occ_now)
                && checkSol(t->p_sol,t->max_rip,pos))
                    update_best(t,pos);
        return;
    }
    for(i=0; i<e_void; i++){
        if(t->pietre[i].occ_now < t->pietre[i].num && (pos==0 || !pruning(t,pos,i)) ) {
            t->pietre[i].occ_now++;
            if(pos == 0 || t->p_sol.sol[pos-1]==t->pietre[i].type)
                t->pietre[i].rip_now++;
            t->p_sol.sol[pos] = t->pietre[i].type;
            t->p_sol.value += t->pietre[i].val;
            dispRip(t, pos + 1);
            t->p_sol.value -= t->pietre[i].val;
            if(t->pietre[i].rip_now > 0)
                t->pietre[i].rip_now--; /*Cambio pietra*/
            t->pietre[i].occ_now--;
        }else if((t->p_sol.value > t->best_sol.value)
                    &&(t->pietre[e_zaffiro].occ_now <= t->pietre[e_smeraldo].occ_now)
                        && checkSol(t->p_sol,t->max_rip,pos))
                            update_best(t,pos);
    }
}
void update_best(tab_t *t,int n){
    int i;
    t->best_sol.value = t->p_sol.value;
    t->best_sol.n = n;
    if (t->best_sol.sol != NULL) free(t->best_sol.sol);
    t->best_sol.sol = malloc(t->best_sol.n * sizeof(e_tipo));
    for ( i = 0; i < t->best_sol.n; i++)
        t->best_sol.sol[i] = t->p_sol.sol[i];
}
int pruning (tab_t *t, int pos, int i){
/*i indice esaminato*/
    e_tipo prev = t->p_sol.sol[pos-1];

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

int checkSol (Sol test_sol,int max_rip,int n){
    int i = 1,check=1;
    e_tipo test = test_sol.sol[0];
    while (i<n){
        if(test_sol.sol[i]==test) check++;
        else check = 1;
        if(check > max_rip) return 0;
        test = test_sol.sol[i];
        i = i+1;
    }
    return 1;
}


void stampa(tab_t *t){
    int i;
    char dict[e_void] = {'z','r','t','s'};
    printf("ZAFFIRO[%d] %d, RUBINO[%d] %d, TOPAZIO[%d]  %d, SMERALDO[%d]  %d, TOT:%d, {max_rip: %d}\nSoluzione ottima di valore %d usando %d gemma/e\nComposizione collana: ",
           t->pietre[e_zaffiro].val,t->pietre[e_zaffiro].num,t->pietre[e_rubino].val,t->pietre[e_rubino].num,t->pietre[e_topazio].val,
           t->pietre[e_topazio].num,t->pietre[e_smeraldo].val, t->pietre[e_smeraldo].num,t->n_max,t->max_rip,t->best_sol.value,t->best_sol.n);
    for(i=0; i<t->best_sol.n; i++)
        printf("%c",dict[t->best_sol.sol[i]]);
}

void free_tab(tab_t *DB){
    free(DB->pietre);
    free(DB->best_sol.sol);
}

