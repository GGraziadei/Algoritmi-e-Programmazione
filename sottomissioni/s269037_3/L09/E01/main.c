#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define S 50+1
typedef int time;
typedef struct{
    time inizio,fine;
    int rest_time;
}att;
typedef struct {
    int value,card;
    att **sol; /*Vettore di puntatori ad attività*/
}sol;
int init_att(att **a);
void attSel(int N,att *v);
int comb_sempl(int N,int pos,int start, sol *p_sol, sol *best,att *v);
void stampaSol(FILE *out,sol *s);
void updateBest (sol *b, sol *p);
int countValue(att **s, int card);
static char buf[S];
int main() {
    setbuf(stdout, 0);
    att *v;
    int N = init_att(&v);
    attSel(N,v);
    free(v);
    return 0;
}
void attSel(int N,att *v){
    /*Power Set tramite modello delle combinazioni semplici, applico la ricerca esponeziale
     * per la ricerca della cardinalità ottima*/
    int res;
    sol bestSol,p_sol;
    bestSol.value = p_sol.value = 0;
    bestSol.sol = NULL;
    for(p_sol.card=1; p_sol.card<=N && res>0; p_sol.card++){
        p_sol.sol = (att**)malloc(p_sol.card*sizeof(att*));
        res = comb_sempl(N,0,0,&p_sol,&bestSol,v);
        free(p_sol.sol);
    }
    stampaSol(stdout,&bestSol);
    free(bestSol.sol);
}

int comb_sempl(int N,int pos,int start, sol *p_sol, sol *best,att *v){
    int i,count = 0;
    if(pos >= p_sol->card){
        //stampaSol(stdout,p_sol);
        p_sol->value = countValue(p_sol->sol,p_sol->card);
        if(p_sol->value > best->value){
            updateBest (best,p_sol);
            return 1;
        }
        return 0;
    }
    for(i=start; i<N; i++){
        if(pos == 0 ||  v[i].inizio >= p_sol->sol[pos-1]->fine ){
            p_sol->sol[pos] = &(v[i]);
           if(countValue(p_sol->sol,pos+1) + v[i].rest_time > best->value)
                /*Criteri di pruning:
                 * 1. Compatibilità della soluzione con le spcifiche
                 * 2. Ricorro <=> la soluzione che al più posso combinare ha la speranza di superare, per criterio di ottimalità,
                 * l' attuale best, al netto della eventuali incompatibilità*/
                count += comb_sempl(N,pos+1,i+1,p_sol,best,v);
        }
    }
    return count;
}
int countValue(att **s, int card){
    int i,val=0;
    for(i=0; i<card; i++)
        val += (s[i]->fine - s[i]->inizio);
    return val;
}
void updateBest (sol *b, sol *p){
    int i;
    if(b->sol != NULL)
        free(b->sol);
    b->value = p->value;
    b->card = p->card;
    b->sol = (att**) malloc(b->card * sizeof(att*));
    for(i=0; i< b->card; i++)
        b->sol[i] = p->sol[i];
}
void stampaSol(FILE *out,sol *s){
    int i;
    fprintf(out,"CARDINALITA' %d - VALORE %d \nS={ ",s->card,s->value);
    for (i = 0; i < s->card; i++)
        fprintf(out,"(%d, %d), ",s->sol[i]->inizio,s->sol[i]->fine);
    fprintf(out,"}.");
}
int init_att(att **a){
    int N=0,i,rest_time=0;
    FILE *in = NULL;
    do {
        printf("Inserire percorso file valido: ");
        scanf("%s",buf);
        in = fopen(buf,"r");
        if(in == NULL)printf("Percorso file non valido");
    }while (in == NULL);
    fscanf(in,"%d",&N);
    (*a) = malloc(N*sizeof(att));
    for(i=0; i<N; i++) {
        fscanf(in, "%d %d", &(*a)[i].inizio, &(*a)[i].fine);
        rest_time += (*a)[i].fine - (*a)[i].inizio;
    }
    (*a)[0].rest_time = rest_time - ((*a)[0].fine - (*a)[0].inizio);
    for(i=1; i<N; i++)
        (*a)[i].rest_time = (*a)[i-1].rest_time - ((*a)[i].fine - (*a)[i].inizio);
    fclose(in);
    return N;
}

