#include "GRAPH.h"
typedef struct node *link;
struct node{
    int v,wt;
    link next;
};
struct graph {
    int V,E,**madj; /*Matrice adj*/
    ST st_tab; /*Tabella dei simboli*/
    link z,*ladj;/*Vettore di liste adj*/
};

static int **init_madj(int r,int c, int init_value);
static void free_madj(int r, int **madj);
static EDGE EDGE_create (int id1,int id2, int wt);
static void insertEdge (GRAPH G, EDGE e);
static int isAdj(GRAPH G,int id1, int id2);
static link new_node(int v,int wt,link next);
static void free_node(link node_x);
static link lista_free(link x, link sentinella);

GRAPH GRAPH_init(int V){
    int i;
    GRAPH G = malloc(sizeof (struct graph));
    assert(G != NULL);
    G->V = V;
    G->E = 0;
    G->madj = init_madj(V,V,MADJ_initValue);
    G->ladj = calloc(V,sizeof(link)); /*Inizializza teste liste di adj a NULL*/
    G->st_tab = ST_init(V,ST_initValue);
    G->z = new_node(-1,0,NULL); /*nodo sentinella*/
    for(i=0; i<V; i++)
        G->ladj[i] = G->z;
    return G;
}
int GRAPH_getIndex(GRAPH G, ST_KEY chiave){
    int id;
    id = ST_search(G->st_tab,chiave);
    if (id == -1)
        ST_insert(G->st_tab,ST_count(G->st_tab),chiave);
    return id;
}
static link new_node(int v,int wt,link next){
    link node_x = malloc(sizeof (struct node));
    assert(node_x != NULL);
    node_x->v = v;
    node_x->wt = wt;
    node_x->next = next;
    return node_x;
}
static void free_node(link node_x){
    free(node_x);
}
static void insertEdge (GRAPH G, EDGE e){
    /*Check multigrafo e cappio in grafo non orientato*/
    if(G->madj[e.u][e.v] == MADJ_initValue && e.u != e.v ) {
        G->madj[e.u][e.v] = G->madj[e.v][e.u] = e.wt;
        G->E++;
    }
}
static void insertEdgeList (GRAPH G, EDGE e){
    /*inserzione in testa della lista*/
    G->ladj[e.u] = new_node(e.v,e.wt,G->ladj[e.u]);
    G->ladj[e.v] = new_node(e.u,e.wt,G->ladj[e.v]);
}
static void removeEdge(GRAPH G, EDGE e){
    if(G->madj[e.u][e.v] != MADJ_initValue)
        G->E--;
    G->madj[e.u][e.v] = MADJ_initValue;
    G->madj[e.v][e.u] = MADJ_initValue;
}
void GRAPH_edges(GRAPH G, EDGE *vp){
    int i,j,E=0;
    for(i=0; i<G->V; i++)
        for(j=i+1; j<G->V; j++)
            vp[E++] = EDGE_create(i,j,G->madj[i][j]);
}
static EDGE EDGE_create (int id1,int id2, int wt){
    EDGE e;
    e.v = id1;
    e.u = id2;
    e.wt = wt;
    return e;
}
EDGE EDGE_VOID (){
    EDGE e;
    e.v = -1;
    e.u = -1;
    e.wt = 0;
    return e;
}
void GRAPH_free(GRAPH graph){
    int i;
    free_madj(graph->V,graph->madj);
    ST_free(graph->st_tab);
    for(i=0; i<graph->V; i++){
        graph->ladj[i] = lista_free(graph->ladj[i],graph->z);
        assert(graph->ladj[i] == NULL);
    }
    free(graph->ladj);
    free(graph->z);
    free(graph);
}
static link lista_free(link x, link sentinella){
    link t;
    while (x != sentinella){
        t = x;
        x = x->next;
        free_node(t);
    }
    return x->next;
}
static int **init_madj(int r,int c, int init_value){
    int i,j,**madj;
    madj = (int **)malloc(r*sizeof(int *));
    assert(madj != NULL);
    for(i=0; i<r; i++){
        madj[i] = (int*) malloc(c*sizeof(int));
        assert(madj[i] != NULL);
        for(j=0; j<c; j++)
            madj[i][j] = init_value;
    }
    return madj;
}

static void free_madj(int r, int **madj){
    int i;
    for(i=0; i<r; i++)
        free(madj[i]);
    free(madj);
}
static int isAdj(GRAPH G,int id1, int id2){
    return (G->madj[id1][id2] || G->madj[id2][id1]);
}

