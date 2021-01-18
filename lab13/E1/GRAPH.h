#ifndef E1_GRAPH_H
#define E1_GRAPH_H
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include "ST.h"
#define S 30+1
#define NOT_ADJ -100
typedef struct graph_s *GRAPH;
typedef struct { int v,u,wt; }EDGE;
GRAPH GRAPH_init(int V);
void GRAPH_free(GRAPH G);
EDGE EDGE_create(int u,int v,int wt);
void GRAPH_load(FILE *fin,GRAPH G);
void GRAPH_storage(FILE *fout,GRAPH G);
void EDGES_extract(GRAPH G, EDGE *array);
int GRAPH_GETindex(GRAPH G, ST_key chiave);
void DAG_build(GRAPH G);
void DAG_maxPath(GRAPH D);
#endif //E1_GRAPH_H
