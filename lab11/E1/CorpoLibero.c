#include "CorpoLibero.h"
#define DBG 0
struct elementi{
    int N;
    ELMENT *elementi;
    DIAGLIST diagList;
};
static void dispRipR(ELMENTS eA,int pos,DIAG *DIAG_tmp,int DD);
static int pruning(DIAG *DIAG_tmp,int pos, int indice,int DD,ELMENTS eA);
static int DIAG_check(DIAG *dp,int N,ELMENTS eA);
ELMENTS ELMENTS_init(int N){
    ELMENTS eA = malloc(sizeof (struct elementi));
    eA->elementi = malloc(N*sizeof (ELMENT));
    eA->diagList = DIAGLIST_init();
    eA->N = N;
    return eA;
}
void ELMENTS_read(FILE *fp,ELMENTS eA){
    assert(fp != NULL);
    int N = eA->N;
    eA->N = 0;
    while (eA->N < N){
        eA->elementi[eA->N] = ELMENT_init();
        ELMENT_read(fp,&eA->elementi[eA->N++]);
    }
    assert(eA->N == N);
}
void ELMENTS_print(FILE *out,ELMENTS eA){
    int i;
    fprintf(out,"\n\n");
    for(i=0; i< eA->N; i++){
        fprintf(out,"%d > ",i);
        ELMENT_print(out,&eA->elementi[i]);
    }
}

void ELMENTS_free(ELMENTS eA){
    int i;
    for(i=0; i<eA->N; i++)
        ELMENT_free(eA->elementi[i]);
    DIAGLIST_free(eA->diagList);
    free(eA);
}
ELMENT ELMENTS_getELMENT(ELMENTS eA,int index){
    return (eA->elementi[index]);
}


void CORPOLIBERO_creaDiagList(ELMENTS eA,int DD){
    DIAG DIAG_tmp = DIAG_void();
    dispRipR(eA,0,&DIAG_tmp,DD);
    DIAG_free(&DIAG_tmp);
    fprintf(stdout,"\n%d DIAGONALI COMPATIBILI\n",DIAGLIST_getNDiag(eA->diagList));
    #if DBG
    FILE *out = fopen("DIAGLIST.txt","w");
    DIAGLIST_print(out,eA->diagList);
    #endif
}

static void dispRipR(ELMENTS eA,int pos,DIAG *DIAG_tmp,int DD){
    int i;
    DIAG pSol;
    if(pos>=DIAG_num ||
            (pos>0 && eA->elementi[DIAG_tmp->elments_index[pos-1]].parametri_tecnici[p_finale] == e_f_finale))
    {
        /*Condizione di terminazione: ho trovato una diagonale compatibile posizionando 5 elemtni
         * oppure ho trovato un elemento finale*/
        if(DIAG_check(DIAG_tmp,pos,eA)) {
            pSol = DIAG_init(pos, DIAG_tmp->elments_index, DIAG_tmp->value, DIAG_tmp->livello);
            #if DBG
            DIAG_print(stdout,&pSol);
            #endif
            DIAGLIST_add(pSol, eA->diagList);
        }
        /*pSol è la diagonale assegnata al nodo in lista, la sua deallocazione è dipendente dal nodo*/
        return;
    }
    for(i=0; i<eA->N; i++){
        if(!pruning(DIAG_tmp,pos,i,DD,eA)){
            DIAG_tmp->elments_index[pos] = i;
            DIAG_tmp->value += eA->elementi[i].punti;
            DIAG_tmp->livello += eA->elementi[i].livello;
            dispRipR(eA, pos + 1, DIAG_tmp, DD);
            DIAG_tmp->value -= eA->elementi[i].punti;
            DIAG_tmp->livello -= eA->elementi[i].livello;
        }else if(DIAG_check(DIAG_tmp,pos,eA)) {
            pSol = DIAG_init(pos, DIAG_tmp->elments_index, DIAG_tmp->value, DIAG_tmp->livello);
            if(DIAGLIST_isSaved(eA->diagList,&pSol) == 0 ){
                #if DBG
                DIAG_print(stdout,&pSol);
                #endif
                DIAGLIST_add(pSol, eA->diagList);
            }
            else DIAG_free(&pSol);
        }
    }
}

static int pruning(DIAG *DIAG_tmp,int pos, int indice,int DD,ELMENTS eA){
    int idA,idB;
    if(pos > 0)
        idA = DIAG_tmp->elments_index[pos-1];
    idB = indice;
    if(pos == 0) {
        if (eA->elementi[idB].parametri_tecnici[p_d_entrata] != e_d_frontale)
            return 1;
        if (eA->elementi[idB].parametri_tecnici[p_precedenza] == e_p_preceduto)
            return 1;
    }
    else if(eA->elementi[idB].parametri_tecnici[p_d_entrata] != eA->elementi[idA].parametri_tecnici[p_d_uscita])
        return 1;
    if(eA->elementi[idB].livello + DIAG_tmp->livello > DD)
        return 1;
    return 0;
}

static int DIAG_check(DIAG *dp,int N,ELMENTS eA){
    int i;
    e_tipologia e_acrobatico;
    for(i=0; i<N; i++){
        e_acrobatico = eA->elementi[dp->elments_index[i]].parametri_tecnici[p_tipologia];
        if(e_acrobatico == e_t_indietro || e_acrobatico == e_t_avanti)
            return 1;
    }
    return 0;
}

void CORPOLIBERO_creaPROGRAM(ELMENTS eA,int DP){
    PROGRAM program_best = DIAGLIST_makePROGRAM (eA->diagList,DP,eA);
    PROGRAM_print(stdout,&program_best);
    PROGRAM_free(&program_best);
}

