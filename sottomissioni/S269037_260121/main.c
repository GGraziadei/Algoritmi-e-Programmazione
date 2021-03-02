#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#define PROPOSTAvalida "proposta10_4.txt"
#define PROPOSTAnonValida "proposta10_4NV.txt"
#define LABIRINTO "labirintoTestPF.txt"
int main(int argc, char  **argv) {
    setbuf(stdout,0);
    if(argc != 3)exit(-1);
    int M,PF;
    M = atoi(argv[1]);
    PF = atoi(argv[2]);
    FILE *fp;
    fp = fopen(LABIRINTO,"r");
    if(fp == NULL)exit(-1);
    Graph g = GRAPHload(fp);
    fclose(fp);
    GRAPHstore(g,stdout);
    fp = fopen("proposta10_4.txt","r"); /*Proposta valida per M = 10 e PF = 4*/
    if(fp == NULL)exit(-1);
    printf("\n\nPATH:\t %s M = %d PF = %d\t",PROPOSTAvalida,M,PF); GRAPHpathLoad(g,fp,M,PF);
    fclose(fp);
    fp = fopen(PROPOSTAnonValida,"r"); /*Proposta non valida per M = 10 e PF = 4*/
    if(fp == NULL)exit(-1);
    printf("PATH:\t %s M = %d PF = %d\t",PROPOSTAnonValida,M,PF); GRAPHpathLoad(g,fp,M,PF);
    fclose(fp);
    printf("\n##PATH OTTIMO: \n");
    GRAPHpathBest(g,M,PF);
    GRAPHfree(g);
    return 0;
}
