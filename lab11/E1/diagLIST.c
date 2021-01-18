#include "diagLIST.h"

typedef struct node *link;
struct node{
    DIAG d;
    link next;
};
struct diagList{
    link head;
    int nDiag;
};
static link liberaLista(link h);
static void freeNode(link x);
static link new_node(DIAG d,link next);
static link LinearSearch(DIAGLIST dl,DIAG *dp);
static void dispRipR(link head,int pos,PROGRAM *p, PROGRAM *best,int DP,ELMENTS eA);
static float getBonus(DIAG *dp,ELMENTS eA);
static int check(PROGRAM *p);
DIAGLIST DIAGLIST_init(){
    DIAGLIST dl = malloc(sizeof (struct diagList));
    dl->head = NULL;
    dl->nDiag = 0;
    return dl;
}
static link liberaLista(link h){
    link tmp = h;
    if (h == NULL )return NULL;
    while(h != NULL){
        tmp = h->next;
        freeNode(h);
        h = tmp;
    }
    assert(h == NULL);
    return h;
}
void DIAGLIST_free(DIAGLIST dl){
    dl->head = liberaLista(dl->head);
    free(dl);
}

static link new_node(DIAG d,link next){
    link node_x = malloc(sizeof (struct node));
    assert(node_x != NULL);
    node_x->next = next;
    node_x->d = d;
    return node_x;
}
static void freeNode(link x){
    DIAG_free(&x->d);
    free(x);
}

void DIAGLIST_add(DIAG d, DIAGLIST dl){
    /*Inserzione in testa*/
    dl->head = new_node(d,dl->head);
    dl->nDiag++;
}
static link LinearSearch (DIAGLIST dl,DIAG *dp){
    link x = dl->head;
    while (x != NULL && !DIAG_eq(dp,&x->d)) x = x->next;
    return x;
}
int DIAGLIST_getNDiag(DIAGLIST dl){
    return dl->nDiag;
}

void DIAGLIST_print(FILE *out,DIAGLIST dl){
    link x = dl->head;
    while (x!=NULL){
        DIAG_print(out,&x->d);
        x = x->next;
    }
}

PROGRAM DIAGLIST_makePROGRAM (DIAGLIST dl,int DP,ELMENTS eA){
    PROGRAM best_program,p_program;
    best_program = PROGRAM_void();
    p_program = PROGRAM_void();
    if(dl->head == NULL){
        printf("ERRORE\n");
        return PROGRAM_void();
    }
    dispRipR(dl->head,0,&p_program,&best_program,DP,eA);
    PROGRAM_free(&p_program);
    return best_program;
}

/*Non essendoci alcun vincolo di concatenazione tra due diagonali è possibile valutare soluzioni le cui prime due diagonali
 * siano invertite rispetto le soluzioni proposte, tutto dipende dall'ordine quindi criteri di pruning durante la generazione diagonali. */
static void dispRipR(link head,int pos,PROGRAM *p, PROGRAM *best,int DP,ELMENTS eA){
    link x;
    if(pos >= DIAG_IN_PROGRAM ){
        p->bonus = getBonus(&p->programmaGara[DIAG_IN_PROGRAM-1],eA);
        if(DIAGLIST_checkPROGRAM(p,eA,DP) == 1) {
            if ((p->value + p->bonus) > best->value) {
                /*Eseguo per prima la check program con lo scopo duplice di ridurre gli errori legati al calcolo su float che
                 * si perturbano su somme e differenze ripetute e testare il programma di gara*/
                PROGRAM_cpy(best, p);
                best->value += p->bonus;
            }
        }
        return;
    }
    for(x = head; x!=NULL; x = x->next){
        if( (p->livello + x->d.livello) <= DP) {
            if (p->programmaGara[pos].elments_index != NULL)
                DIAG_free(&p->programmaGara[pos]);
            DIAG_cpy(&p->programmaGara[pos],&(x->d));/*Copia tutte le informazioni sulla diagonale*/
            p->value += p->programmaGara[pos].value;
            p->livello += p->programmaGara[pos].livello;
            dispRipR(head, pos + 1, p, best, DP,eA);
            p->value -= p->programmaGara[pos].value;
            p->livello -= p->programmaGara[pos].livello;
            /*NOTA: Sommare e sottrarre float produce un errore non trascurabile*/
        }
    }
}

int DIAGLIST_checkPROGRAM(PROGRAM *p,ELMENTS eA,int DP){
    int i,j,checkDoubleA=0,checkAA=0,checkAI=0;
    DIAG d = DIAG_void();
    ELMENT e,e_tmp;
    p->value = 0;
    for(i=0; i<DIAG_IN_PROGRAM; i++){
        if(d.elments_index != NULL) DIAG_free(&d);
        p->value += p->programmaGara[i].value;
        DIAG_cpy(&d,&p->programmaGara[i]);
        for(j=0; j<d.num; j++){
            e = ELMENTS_getELMENT(eA,d.elments_index[j]);
            if(e.parametri_tecnici[p_tipologia] == e_t_indietro) checkAI = 1;
            if(e.parametri_tecnici[p_tipologia] == e_t_avanti ) checkAA = 1;
            if(j>0 && ! checkDoubleA){
                e_tmp = ELMENTS_getELMENT(eA,d.elments_index[j-1]);
                if(e.parametri_tecnici[p_tipologia] != e_t_transizione &&
                        e_tmp.parametri_tecnici[p_tipologia] != e_t_transizione)
                            checkDoubleA = 1;
            }
        }
    }
    DIAG_free(&d);
    return (checkAA*checkAI*checkDoubleA);
}
static float getBonus(DIAG *dp,ELMENTS eA){
    int indice = dp->elments_index[dp->num-1];
    ELMENT last = ELMENTS_getELMENT(eA,indice);
    if (last.livello>=8)
        return (dp->value / 2);
    return 0;
}
