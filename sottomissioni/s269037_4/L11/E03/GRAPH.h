#ifndef E3_GRAPH_H
#define E3_GRAPH_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ST.h"
#define MADJ_initValue 0
enum { falso, vero };
typedef int bool;
typedef enum {madj,ladj}source_info;
typedef struct graph *GRAPH;
typedef struct { int u,v,wt; }EDGE;
GRAPH GRAPH_init(int V, ST st_tab);
EDGE EDGE_VOID ();
void GRAPH_edges(GRAPH G, EDGE *vp);
int GRAPH_getIndex(GRAPH G, ST_KEY chiave, ST_KEY net);
void GRAPH_free(GRAPH graph);
void GRAPH_store(FILE *out,GRAPH G);
void GRAPH_read(FILE *in,GRAPH G);
void GRAPH_lAdj(GRAPH G);
bool GRAPH_isAdj(GRAPH G,ST_KEY key1,ST_KEY key2,source_info source);
void GRAPH_info(FILE *out, GRAPH G,source_info source);
bool GRAPH_isLadj(GRAPH G);
#endif //E3_GRAPH_H
