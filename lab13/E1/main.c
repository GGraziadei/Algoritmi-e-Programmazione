#include <stdio.h>
#include "GRAPH.h"
#define GRAFO "grafo.txt"

int main() {
    setbuf(stdout,0);
    int N;
    FILE *fin = fopen(GRAFO,"r");
    assert(fin != NULL);
    fscanf(fin,"%d",&N);
    GRAPH G = GRAPH_init(N);
    GRAPH_load(fin,G);
    fclose(fin);
    printf("\nGrafo di partenza \n");
    GRAPH_storage(stdout,G);
    printf("\nEnumerazione di set di archi che rimossi rendono il grafo un DAG \n");
    DAG_build(G);
    printf("DAG: \n");
    GRAPH_storage(stdout,G);
    printf("\nDistanza massima da ogni nodo sorgente:\n");
    DAG_maxD(G);
    GRAPH_free(G);
    return 0;
}
