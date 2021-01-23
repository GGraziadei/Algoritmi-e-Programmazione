#include "CorpoLibero.h"
#define DBG 0
struct elementi{
    int N;
    ELMENT *elementi;
};
static void Merge(ELMENT *A,int inf,int q,int sup,ELMENT *B);
static void MergeSortR(ELMENT *eA,int inf,int sup, ELMENT *B);
static int DIAG_check(DIAG *dp,int N,int DD);
static float getBonus(DIAG *dp);
static void checkProgram(PROGRAM *p,int DD,int DP);

ELMENTS ELMENTS_init(int N){
    ELMENTS eA = malloc(sizeof (struct elementi));
    eA->elementi = malloc(N*sizeof (ELMENT));
    /* eA->diagList = DIAGLIST_init(); */
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
    /* DIAGLIST_free(eA->diagList);*/
    free(eA);
}
ELMENT ELMENTS_getELMENT(ELMENTS eA,int index){
    return (eA->elementi[index]);
}

void ELMENTS_sort(ELMENTS eA){
    ELMENT *B = malloc(eA->N * (sizeof (ELMENT)));
    /*Copio il puntatore a memoria allocata dinamicamente, quindi non devo deallocare
     * gli ELMENT di questo array*/
    MergeSortR(eA->elementi,0,eA->N-1,B);
    free(B);
}
static void MergeSortR(ELMENT *eA,int inf,int sup, ELMENT *B){
    if(inf >= sup)return;
    int q  = (inf + sup)/2;
    MergeSortR(eA,inf,q,B);
    MergeSortR(eA,q+1,sup,B);
    Merge(eA,inf,q,sup,B);
}
static void Merge(ELMENT *A,int inf,int q,int sup,ELMENT *B){
    int i,j,k;
    i = inf;
    j = q+1;
    for(k=inf; k<=sup; k++){
        if(i > q)
            B[k] = A[j++];
        else if(j>sup)
            B[k] = A[i++];
        else if(A[i].app > A[j].app)
            B[k] = A[i++];
        else
            B[k] = A[j++];
    }
    for(k=inf; k<=sup; k++)
        A[k] = B[k];
}
static void appCalc_starter(ELMENTS eA){
    int i;
    for(i=0; i<eA->N; i++) {
        /*Ordino per appetibilità cercando di distribuire elementi finali in funzione di pos*/
        eA->elementi[i].app = ((eA->elementi[i].punti / (float) eA->elementi[i].livello));
        if (eA->elementi[i].parametri_tecnici[p_precedenza] != e_p_non_preceduto ||
                eA->elementi[i].parametri_tecnici[p_d_entrata] != e_d_frontale) {
            eA->elementi[i].app = -1;
        }
    }
}
static void appCalc_Jekill(ELMENTS eA){
    int i;
    for(i=0; i<eA->N; i++) {
        /*Ordino per appetibilità cercando di distribuire linearmente elementi finali in funzione di pos*/
        eA->elementi[i].app = (eA->elementi[i].punti / (float) eA->elementi[i].livello ) ;
    }
}
static int isAcro(e_tipologia t){
    return (t==e_t_avanti || t==e_t_indietro);
}
static void appCalc_Hyde(ELMENTS eA, e_direzione direzioneHyde, e_tipologia tipologiaHyde){
    int i;
    for(i=0; i<eA->N; i++) {
        /*Ordino per appetibilità cercando di distribuire elementi finali in funzione di pos*/
        eA->elementi[i].app = eA->elementi[i].punti;

        if (eA->elementi[i].parametri_tecnici[p_d_uscita] == direzioneHyde) {
            eA->elementi[i].app = -1;
        }
        if (isAcro(eA->elementi[i].parametri_tecnici[p_tipologia]) && isAcro(tipologiaHyde))
            eA->elementi[i].app = -1;
        if(eA->elementi[i].parametri_tecnici[p_tipologia] == e_t_transizione)
            eA->elementi[i].app = -1;
    }
}
static void appCalc_bottomAcroA(ELMENTS eA){
    int i;
    for(i=0; i<eA->N; i++) {
        /*Ordino per appetibilità cercando di distribuire linearmente elementi finali in funzione di pos*/
        eA->elementi[i].app = eA->elementi[i].punti / (float) eA->elementi[i].livello;
        if(eA->elementi[i].parametri_tecnici[p_tipologia] == e_t_avanti)
            eA->elementi[i].app *= -1;
    }
}
void CORPOLIBERO_greedy(ELMENTS eA,int DD,int DP){
    int i,j,indexAPP,found,pos = 0,prevAcroA = 0;
    PROGRAM p = PROGRAM_void();
    /*La diagonale ha posto un elemento finale ? su prima colonna elementi finali hanno bassa appetibilità*/
    int DIAG_isFinale[DIAG_IN_PROGRAM] = {0};
    /*Dir in uscita last elemento, su prima colonna direzione di entrata frontale*/
    e_direzione DIAG_dir[DIAG_IN_PROGRAM] = {e_d_frontale,e_d_frontale,e_d_frontale};
    /*Tipologia ultimo elemento DIAG, primo inserimento acrobatico avanti*/
    e_tipologia DIAG_tipo[DIAG_IN_PROGRAM] = {e_t_avanti,e_t_avanti,e_t_avanti};

    /*Ho due criteri di appetibilità Jekill ed Hyde, mantengo traccia dello stato per cui ho calcolato
     * l'appetibilità. Ad un certo punto, sono costretto a penalizzare elementi acro avanti.*/
    int Hyde = 0;

    /*Ordinamento per elemento starter*/
    appCalc_starter(eA);
    ELMENTS_sort(eA);
    indexAPP = 0;

    /*Inserimento "per colonne"*/
    for(j=0; j<DIAG_num; j++){
        for(i=0; i<DIAG_IN_PROGRAM; i++){
            found = 0;
            /*Verifico appetibilità elemento */
            if(DIAG_isFinale[i] == 0){
                while (!Hyde && !found && indexAPP < (eA->N) && eA->elementi[indexAPP].app >= 0 ) {
                    if(p.programmaGara[i].livello + eA->elementi[indexAPP].livello <= DD)
                        if(p.livello + eA->elementi[indexAPP].livello <= DP)
                            if(DIAG_dir[i] == eA->elementi[indexAPP].parametri_tecnici[p_d_entrata])
                                found = 1;
                    if(!found) indexAPP++;
                }
                if(!found){
                    /*Favorisco la variazione di direzione in uscita */
                    Hyde = 1;
                    appCalc_Hyde(eA,DIAG_dir[i],DIAG_tipo[i]);
                    ELMENTS_sort(eA);

                    /*Questo implica che ho ottenuto necessariamente un elemento appetibile in index 0 */
                    indexAPP = 0;
                    found = 1;
                    #if DBG
                    ELMENTS_print(stdout, eA);
                    PROGRAM_print(stdout, &p);
                    #endif
                }
                if(p.programmaGara[i].livello + eA->elementi[indexAPP].livello <= DD &&
                    p.livello + eA->elementi[indexAPP].livello <= DP &&
                        DIAG_dir[i] == eA->elementi[indexAPP].parametri_tecnici[p_d_entrata]) {
                            if (isAcro(DIAG_tipo[i])) prevAcroA += 1;
                            p.livello += eA->elementi[indexAPP].livello;
                            DIAG_dir[i] = eA->elementi[indexAPP].parametri_tecnici[p_d_uscita];
                            DIAG_tipo[i] = eA->elementi[indexAPP].parametri_tecnici[p_tipologia];
                            DIAG_addE(&p.programmaGara[i], &eA->elementi[indexAPP++]);
                            if (DIAG_tipo[e_t_avanti] && prevAcroA >= 2 && pos <= DIAG_IN_PROGRAM) {
                                /*Empiricamente confrontando la soluzione ottenuta con il calcolo comb, rimepiendo per colonne dopo una
                                 * frequenza di due acro avanti è necessario forzare un elemento indietro
                                 * per evitare che la saturazione di DP inneschi il processo di variazione dir troppo tardi. */
                                appCalc_bottomAcroA(eA);
                                ELMENTS_sort(eA);
                                Hyde = prevAcroA = 0;
                            }
                            pos++;
                        }
            }
            if (Hyde){
                Hyde = 0;
                appCalc_Jekill(eA);
                ELMENTS_sort(eA);
                indexAPP = 0;
                #if DBG
                ELMENTS_print(stdout, eA);
                PROGRAM_print(stdout, &p);
                #endif
            }
        }

        if(j == 0 && !Hyde){
            /*Superata la prima colonna 'starter' varia il criterio di appetibilità*/
            Hyde = 1;
            appCalc_Jekill(eA);
            ELMENTS_sort(eA);
            indexAPP = 0;
        }
    }
    checkProgram(&p,DD,DP);
    PROGRAM_free(&p);
}

static void checkProgram(PROGRAM *p,int DD,int DP){
    int i,bestD;
    float bestV = 0;
    DIAG tmp;
    /*Verifica diagonali e programma, scambio di diagonali se è possibile aumentare il bonus sulla diagonale finale*/
    for(i=0; i<DIAG_IN_PROGRAM
             && DIAG_check(&p->programmaGara[i],p->programmaGara[i].num,DD); i++){
        if(bestV < p->programmaGara[i].value){
            bestV = p->programmaGara[i].value;
            bestD = i;
        }
        p->value += p->programmaGara[i].value;
    }
    if(bestD != DIAG_IN_PROGRAM - 1 )
        if(p->programmaGara[bestD].e_inDiag[p->programmaGara[bestD].num-1].livello > 8
           && p->value + getBonus(&p->programmaGara[bestD]) < DP){
            DIAG_cpy(&tmp,&p->programmaGara[DIAG_IN_PROGRAM - 1]);
            DIAG_cpy(&p->programmaGara[DIAG_IN_PROGRAM - 1],&p->programmaGara[bestD]);
            DIAG_cpy(&p->programmaGara[bestD],&tmp);
        }
    if(i == DIAG_IN_PROGRAM){
        p->bonus = getBonus(&p->programmaGara[DIAG_IN_PROGRAM - 1]);
        if(p->bonus){
            printf("BONUS elaborato\n");
            p->value += p->bonus;
        }
        if(CORPOLIBERO_checkPROGRAM(p,DP))
            PROGRAM_print(stdout,p);
        else printf("ERRORE CHECK PROGRAMMA");
    }
    else printf("ERRORE CHECK DIAGONALI");
}
static int DIAG_check(DIAG *dp,int N,int DD){
    int i;
    e_tipologia e_acrobatico;
    if(dp->livello > DD)
        return 0;
    for(i=0; i<N; i++){
        e_acrobatico = dp->e_inDiag[i].parametri_tecnici[p_tipologia];
        if(e_acrobatico == e_t_indietro || e_acrobatico == e_t_avanti)
            return 1;
    }
    return 0;
}
int CORPOLIBERO_checkPROGRAM(PROGRAM *p,int DP){
    int i,j,checkDoubleA=0,checkAA=0,checkAI=0;
    if(p->livello > DP)
        return 0;
    for(i=0; i<DIAG_IN_PROGRAM && (checkAI*checkAA*checkDoubleA == 0); i++){
        for(j=0; j<p->programmaGara[i].num; j++){
            if(p->programmaGara[i].e_inDiag[j].parametri_tecnici[p_tipologia] == e_t_indietro) checkAI = 1;
            if(p->programmaGara[i].e_inDiag[j].parametri_tecnici[p_tipologia] == e_t_avanti ) checkAA = 1;
            if(j>0 && ! checkDoubleA){
                if(p->programmaGara[i].e_inDiag[j].parametri_tecnici[p_tipologia] != e_t_transizione &&
                        p->programmaGara[i].e_inDiag[j-1].parametri_tecnici[p_tipologia] != e_t_transizione)
                    checkDoubleA = 1;
            }
        }
    }
    return (checkAA*checkAI*checkDoubleA);
}
static float getBonus(DIAG *dp){
    if (dp->e_inDiag[dp->num-1].livello>=8)
        return (dp->value / 2);
    return 0;
}




