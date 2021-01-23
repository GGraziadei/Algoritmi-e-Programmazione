#include "GRAPH.h"
#define S 30+1
typedef struct node *link;
struct node{
    int v,wt;
    link next;
};
struct graph {
    int V,E,**madj; /*Matrice adj*/
    ST st_tab,net_tab; /*Tabella dei simboli*/
    link z,*ladj;/*Vettore di liste adj*/
    bool ladjIs;
};

static int **init_madj(int r,int c, int init_value);
static void free_madj(int r, int **madj);
static EDGE EDGE_create (int id1,int id2, int wt);
static void insertEdge (GRAPH G, EDGE e);
static void insertEdgeList (GRAPH G, EDGE e);
static int isAdj(GRAPH G,int id1, int id2);
static link new_node(int v,int wt,link next);
static void free_node(link node_x);
static link lista_free(link x, link sentinella);
static int isAdjL(GRAPH G,int id1, int id2);

GRAPH GRAPH_init(int V,ST st_tab){
    int i;
    GRAPH G = malloc(sizeof (struct graph));
    assert(G != NULL);
    G->V = V;
    G->E = 0;
    G->madj = init_madj(G->V,G->V,MADJ_initValue);
    G->ladj = calloc(V,sizeof(link)); /*Inizializza teste liste di adj a NULL*/
    G->st_tab = st_tab;
    G->z = new_node(-1,0,NULL); /*nodo sentinella*/
    for(i=0; i<V; i++)
        G->ladj[i] = G->z;
    G->ladjIs = falso;
    return G;
}
int GRAPH_getIndex(GRAPH G, ST_KEY chiave, ST_KEY net){
    int id;
    id = ST_search(G->st_tab,chiave);
    if (id == -1)
        ST_insert(G->st_tab,chiave,net);
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
          if(G->madj[i][j] != MADJ_initValue)
              vp[E++] = EDGE_create(i,j,G->madj[i][j]);
}
static EDGE EDGE_create (int id1,int id2, int wt){
    EDGE e;
    e.u = id1;
    e.v = id2;
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
    free_node(graph->z);
    free(graph);
}
static link lista_free(link x, link sentinella){
    link t;
    while (x != sentinella){
        t = x;
        x = x->next;
        free_node(t);
    }
    /*La deallocazione del nodo sentinella è demandata al suo owner ovvero il GRAFO*/
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
static int isAdjL(GRAPH G,int id1, int id2){
    link t;
    for(t=G->ladj[id1]; t!=G->z; t = t->next)
        if(t->v == id2)
            return 1;
    return 0;
}
bool GRAPH_isLadj(GRAPH G){
    return G->ladjIs;
}
void GRAPH_store(FILE *out,GRAPH G){
    int i;
    EDGE *archi;
    fprintf(out,"%d\n",G->V);
    for(i=0; i<G->V; i++)
        fprintf(out,"%30s\t[%s]\n" ,ST_getKey(G->st_tab,i).chiave,ST_getKey(G->st_tab,i).net);
    if(G->E > 0){
        archi = malloc(G->E*sizeof (EDGE));
        GRAPH_edges(G,archi);
        for(i=0; i<G->E; i++)
            fprintf(out,"%30s \t[%s] %30s \t[%s] %2d\n",ST_getKey(G->st_tab,archi[i].u).chiave,ST_getKey(G->st_tab,archi[i].u).net,
                    ST_getKey(G->st_tab,archi[i].v).chiave,ST_getKey(G->st_tab,archi[i].v).net,G->madj[archi[i].u][archi[i].v]);
    }
}
void GRAPH_read(FILE *in,GRAPH G){
    assert(in != NULL);
    int wt;
    char elab1[S],elab2[S],rete1[S],rete2[S];
    while(fscanf(in,"%s %s %s %s %d",elab1,rete1,elab2,rete2,&wt) == 5)
        insertEdge(G,EDGE_create(ST_search(G->st_tab,elab1),ST_search(G->st_tab,elab2),wt));
}
void GRAPH_lAdj(GRAPH G){
    int i;
    if(G->ladjIs == vero)return;
    EDGE *archi = malloc(G->E*sizeof(EDGE));
    GRAPH_edges(G,archi);
    for(i=0; i<G->E; i++)
        insertEdgeList(G,archi[i]);
    G->ladjIs = vero;
    free(archi);
}
bool GRAPH_isAdj(GRAPH  G,ST_KEY key1,ST_KEY key2,source_info source){
    int id1 = ST_search(G->st_tab,key1) , id2 = ST_search(G->st_tab,key2);
    assert(source<=G->ladjIs);
    if(id1 != -1 && id2 != -1){
        if(source == madj) return isAdj(G,id1,id2);
        return isAdjL(G,id1,id2);
    }
    return falso;
}

void GRAPH_info(FILE *out, GRAPH G, source_info source){
    int i,j;
    link t;
    assert(source<=G->ladjIs);
    for(i=0; i<G->V; i++ ) {
        fprintf(out, "Vertice: %s [%s]\n",
                ST_getKey(G->st_tab, i).chiave, ST_getKey(G->st_tab, i).net);
        if(source == madj) {
            for (j = 0; j < G->V; j++) {
                if (G->madj[i][j])
                    fprintf(out, "\t%s [%s] - %s [%s]\t(wt: %d)\n\n",
                            ST_getKey(G->st_tab, i).chiave, ST_getKey(G->st_tab, i).net,
                            ST_getKey(G->st_tab, j).chiave, ST_getKey(G->st_tab, j).net, G->madj[i][j]);
            }
        }
        else{
            for(t = G->ladj[i]; t!=G->z;t =t->next){
                fprintf(out, "\t%s [%s] - %s [%s]\t(wt: %d)\n\n",
                        ST_getKey(G->st_tab, i).chiave, ST_getKey(G->st_tab, i).net,
                        ST_getKey(G->st_tab, t->v).chiave, ST_getKey(G->st_tab, t->v).net, t->wt);
            }
        }
    }
}
