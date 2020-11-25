#include <stdio.h>
#include <stdlib.h>

#define S 50+1
#define SHOW_INFO 0

typedef enum tipo{
    e_zaffiro,e_rubino,e_topazio,e_smeraldo,e_void
} e_tipo;
typedef struct{
    e_tipo type;
    int num,occ_now;
}Item;
typedef struct{
    e_tipo *sol;
    int n;
}Sol;
typedef struct{
    Item *pietre;
    Sol p_sol,best_sol;
    int n_max;
}tab;

tab init_set (FILE *);
void wrapper_dispRip (tab *t, int inf, int sup);
void free_tab(tab *DB);
int pruning (tab *t, int pos, int i);
int dispRip (tab *t, int pos);
void stampa(tab *t);

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
        printf("TEST #%d\n",i+1);
        tab DB = init_set(fp);
        wrapper_dispRip (&DB,0,DB.n_max);
        free_tab(&DB);
    }

    fclose(fp);
    return 0;
}

tab init_set (FILE *fp){
    tab tab_t;
    int j;
    tab_t.pietre = malloc(e_void*sizeof(Item));
    tab_t.n_max = tab_t.best_sol.n = 0;
    tab_t.best_sol.sol = NULL;
    for (int j = 0; j < e_void; j++) {
        fscanf(fp,"%d",&tab_t.pietre[j].num);
        tab_t.n_max += tab_t.pietre[j].num;
        tab_t.pietre[j].type = j;
        tab_t.pietre[j].occ_now = 0;
    }
    return tab_t;
}

void wrapper_dispRip (tab *t, int inf, int sup){
    if(inf>=sup){
        stampa(t);
        return;
    }
    t->p_sol.n = (sup+inf)/2;
    t->p_sol.sol = calloc(t->p_sol.n,sizeof(e_void));
    if(SHOW_INFO) printf("PROVO CON %d\n",t->p_sol.n);
    if(dispRip(t,0)){
        free(t->p_sol.sol);
        wrapper_dispRip(t,t->p_sol.n+1,sup);
    }else{
        free(t->p_sol.sol);
        wrapper_dispRip(t,inf,t->p_sol.n);
    }
}

int dispRip (tab *t, int pos){
    int i,res;
    if(pos >= t->p_sol.n){
        t->best_sol.n = t->p_sol.n;
        if(t->best_sol.sol != NULL)
            free(t->best_sol.sol);
        t->best_sol.sol = malloc(t->best_sol.n*sizeof(e_tipo));
        for (int i = 0; i < t->best_sol.n; i++)
            t->best_sol.sol[i] = t->p_sol.sol[i];
        if(SHOW_INFO)stampa(t);
        return 1;
    }

    for(i=0; i<e_void; i++){
        if(t->pietre[i].occ_now < t->pietre[i].num && (pos==0 || !pruning(t,pos,i) ) ) {
            t->pietre[i].occ_now++;
            t->p_sol.sol[pos] = t->pietre[i].type;
            res = dispRip(t, pos + 1);
            t->pietre[i].occ_now--;
            if(res)
                return 1;
        }
    }
    return 0;
}

int pruning (tab *t, int pos, int i){
/*i indice esaminato*/
    e_tipo prev = t->p_sol.sol[pos-1];
     if(prev == e_zaffiro && (t->pietre[i].type != e_zaffiro) && (t->pietre[i].type != e_rubino))
                return 1;
     if(prev == e_smeraldo && (t->pietre[i].type != e_smeraldo) && (t->pietre[i].type != e_topazio))
                return 1;
     if(prev == e_rubino &&(t->pietre[i].type != e_smeraldo) && (t->pietre[i].type != e_topazio))
                return 1;
     if(prev == e_topazio && (t->pietre[i].type != e_zaffiro) && (t->pietre[i].type != e_rubino))
                return 1;
    return 0;
}
void stampa(tab *t){
    int i;
    char dict[e_void][S] = {"zaffiro","rubino","topazio","smeraldo"};
    for(i=0; i<t->best_sol.n; i++)
        printf("%s  ",dict[t->best_sol.sol[i]]);
    printf("\n");
    printf("ZAFFIRO %d, RUBINO %d, TOPAZIO %d, SMERALDO %d, TOT:%d\nLUNGHEZZA CATENA MAX: %d",t->pietre[e_zaffiro].num,
           t->pietre[e_rubino].num, t->pietre[e_topazio].num, t->pietre[e_smeraldo].num,t->n_max,t->best_sol.n);
    printf("\n");
}

void free_tab(tab *DB){
    free(DB->pietre);
    free(DB->best_sol.sol);
}

