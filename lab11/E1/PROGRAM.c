#include "PROGRAM.h"

PROGRAM PROGRAM_void(){
    PROGRAM p;
    int i;
    for(i=0; i<DIAG_IN_PROGRAM; i++)
        p.programmaGara[i] = DIAG_void();
    p.bonus = 0;
   p.value = 0;
   p.livello =0;
   return p;
}
void PROGRAM_free(PROGRAM *p){
    int i;
    for(i=0; i<DIAG_IN_PROGRAM; i++)
        DIAG_free(&p->programmaGara[i]);
}
void PROGRAM_print(FILE *out,PROGRAM *pp){
    int i;
    fprintf(out,"\n\nPROGRAMMA (PUNTEGGIO %0.2f)\tLIVELLO DIFFICOLTA'%d\nBONUS FINALE %s (%f)\n",pp->value,
            pp->livello,pp->bonus>0?"x 1.5 DIAG FINALE":"NON APPLICABILE",pp->bonus);
    for(i=0;  i<DIAG_IN_PROGRAM; i++)
        DIAG_print(out,&pp->programmaGara[i]);
}
void PROGRAM_cpy(PROGRAM *p1,PROGRAM*p2){
    int i;
    p1->livello = p2->livello;
    p1->value = p2->value;
    p1->bonus = p2->bonus;
    for(i=0; i<DIAG_IN_PROGRAM; i++)
        DIAG_cpy(&p1->programmaGara[i],&p2->programmaGara[i]);
}