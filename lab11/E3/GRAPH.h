#ifndef E3_GRAPH_H
#define E3_GRAPH_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ST.h"
#define MADJ_initValue 0
typedef struct graph *GRAPH;
typedef struct { int u,v,wt; }EDGE;
GRAPH GRAPH_init(int V);
EDGE EDGE_VOID ();
void GRAPH_edges(GRAPH G, EDGE *vp);
int GRAPH_getIndex(GRAPH G, ST_KEY chiave);
void GRAPH_free(GRAPH graph);

#endif //E3_GRAPH_H
