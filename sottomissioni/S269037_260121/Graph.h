#ifndef GRAPH_H
#define GRAPH_H
#define S 20+1
typedef struct edge { int v; int w; int wt; } Edge;
typedef struct graph_s *Graph;
typedef struct path_s *PATH;
typedef struct {char nome[S]; int p,v,o,countP;}stanza;
Graph GRAPHinit(int V);
void  GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
stanza STANZAcrea(char *buf,int p,int v,int wt);
void  GRAPHinsertE(Graph G, int id1, int id2, int wt);
void  GRAPHstore(Graph G, FILE *fout);
void  GRAPHedges(Graph G, Edge *a);
void printSTANZA(stanza *s);
void GRAPHpathBest(Graph g, int M, int PF);
int GRAPHgetIndex(Graph g,char *nome);
PATH GRAPHpathLoad(Graph g, FILE *fp,int M,int PF);
#endif
