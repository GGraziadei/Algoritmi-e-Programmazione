#include "GRAPH.h"
char l1[S],l2[S];
typedef struct node *link;
struct node{
    int v,wt;
    link next;
};
typedef struct {
    int card,peso;
    EDGE *edgesArray; /*Vettore di archi*/
}comb_sol;
struct graph_s{
    int V,E;
    ST tab;
    link *lAdj,z;
};
static int comb_semplR(comb_sol *sol,EDGE *archi,int pos,int start,EDGE *p_sol,int p_peso,GRAPH G);
static comb_sol expSearch (GRAPH G,EDGE *archi,EDGE *p_sol,int inf,int sup);
static int check(EDGE *p_sol,int card,EDGE *archi,GRAPH G);
static int dfs_isDAGr(link *lAdj, link z,int id_starter, int *pre,int *post,int *time);
static void dfs_TS(GRAPH D,int id_starter,int *pre,int *time,int *ts);
static void maxD(GRAPH D,int *ts, int start);
static void update_sol(comb_sol *sol,EDGE *p_sol);
static void getInfoSet(comb_sol *sol,GRAPH G);
static void topologic_inv(int *ts,int N);

static link new_node(int v,int wt,link next){
    link node_x = malloc(sizeof (struct node));
    node_x->v = v;
    node_x->wt = wt;
    node_x->next = next;
    return node_x;
}
static void free_node(link node_x){
    free(node_x);
}
static link list_free(link h,link z){
    link tmp,t = h;
    while(t != z){
        tmp = t;
        t = t->next;
        free_node(tmp);
    }
    return NULL;
}
static link insertInHead(link h,int v,int wt){
    return new_node(v,wt,h);
}
static link removeE(link h,link z,int v){
    link pre,t;
    if ( h == z)
        return z;
    for(t=h, pre =NULL; t != z;pre = t, t = pre->next){
        if(t->v == v){
            if(t == h) h = t->next;
            else pre->next = t->next;
            free_node(t);
            break;
        }
    }
    return h;
}
static comb_sol sol_init(int card){
    comb_sol sol;
    sol.card = card;
    sol.peso = -1;
    sol.edgesArray = (EDGE *) malloc(sol.card * sizeof (EDGE ));
    return sol;
}
static comb_sol sol_null(){
    comb_sol sol;
    sol.card = 0;
    sol.peso = -1;
    sol.edgesArray = NULL;
    return sol;
}
static void sol_free(comb_sol *sol){
    if(sol->edgesArray != NULL)
        free(sol->edgesArray);
}
static void maxD(GRAPH D,int *ts, int start){
    int i,*d,*st;
    link t;
    d = malloc(D->V * sizeof (int ));
    st = malloc(D->V * sizeof (int ));
    for(i=0; i<D->V; i++){
        d[i] = INT_MIN; /*Sto cercando il cammino max di un DAG*/
        st[i] = -1;
    }
    st[ts[start]] = ts[start];
    d[ts[start]] = 0;
    for( i=start; i<D->V; i++){
       for (t = D->lAdj[ts[i]]; t != D->z; t = t->next) {
           if (d[t->v] < d[ts[i]] + t->wt) {
               /*Relaxation inversa*/
               d[t->v] = d[ts[i]] + t->wt;
               st[t->v] = ts[i];
           }
       }
    }
    for(i=0; i<D->V; i++) {
        if (d[i] != INT_MIN) {
            if (i != ts[start])
                printf("\t-> %s [%d] (parent %s)\n",
                       ST_GETkey(D->tab, i), d[i], ST_GETkey(D->tab, st[i]));
        } else printf("\t->%s [NON RAGGIUNTO]\n", ST_GETkey(D->tab, i));
    }
    free(d);
}
void DAG_maxD(GRAPH D){
    int time,*pre,*ts,i,j;
    EDGE *archi = malloc(D->E * sizeof (EDGE));
    EDGES_extract(D,archi);
    if(check(NULL,0,archi,D) != 1 ){
        printf("FUNZIONO SOLAMENTE CON DAG!\n");
        return;
    }
    free(archi);
    pre = malloc(D->V * sizeof (int ));
    ts = malloc(D->V * sizeof (int ));

    for(i=0; i<D->V; i++) {
        /*Cerco tutte le source presenti.*/
        time = 0;
        for(j=0; j<D->V; j++)
            pre[j] = ts[j] = -1;
        dfs_TS(D, i, pre, &time, ts);
        topologic_inv(ts, D->V);
        j = 0;
        while (ts[j] == -1)
            j++; /*Scorro in avanti nel caso ci siano vertici non scoperti*/
        printf("\nDISTANZA MAX TRA %s %s\n",ST_GETkey(D->tab,ts[j]),j==0?":":"(vertice non sorgente!!)");
        printf("\n");
        maxD(D, ts,j);
    }
    free(pre);
    free(ts);
}
void DAG_build(GRAPH G){
    int i,cardMax;
    comb_sol sol;
    EDGE *archi,*p_sol;
    archi = malloc(G->E * sizeof (EDGE));
    EDGES_extract(G,archi);

    if(check(NULL,0,archi,G)){
        printf("Il GRAFO E' UN DAG.\n");
        return;
    } /*IL GRAFO è UN DAG, sostituisce la verifica sul set di cardinalità 0*/
        /*Un DAG connesso ha |V| - 1 archi. Aggiungere un arco significa innescare un ciclo
     * il numero di archi da rimuovere per ottenere un DAG è quindi pari a |E|-(|V|-1) */
    cardMax= G->E - G->V + 1;
    p_sol = malloc(cardMax * sizeof (EDGE));
    sol = expSearch(G,archi,p_sol,1,cardMax );
    getInfoSet(&sol,G);
    for(i=0; i<sol.card; i++){
        G->lAdj[sol.edgesArray[i].u] = removeE(G->lAdj[sol.edgesArray[i].u],G->z,sol.edgesArray[i].v);
        G->E --;
    }
    free(archi);
    free(p_sol);
}

static comb_sol expSearch (GRAPH G,EDGE *archi,EDGE *p_sol,int inf,int sup){
    int q;
    comb_sol solQ,solTmp;
    if(inf > sup)
        return sol_null();
    q = (inf+sup) /2;
    solQ = sol_init(q); /*Se la soluzione non è corretta la deallocazione e rimandata allo stack chiamante*/
    if(comb_semplR(&solQ,archi,0,0,p_sol,0,G) == 1){
        solTmp = expSearch(G,archi,p_sol,inf,q - 1);
        if(solTmp.peso != -1){
            sol_free(&solQ);
            return solTmp;
        }
        else{
            sol_free(&solTmp);
            return solQ;
        }
    }
    else{
        sol_free(&solQ);
        return expSearch(G,archi,p_sol,q+1,sup);
    }

}
static int comb_semplR(comb_sol *sol,EDGE *archi,int pos,int start,EDGE *p_sol,int p_peso,GRAPH G)
{
    int i ,ok = 0;
    if(pos >= sol->card ){
        if (p_peso > sol->peso && check (p_sol,sol->card,archi,G)){
            sol->peso = p_peso;
            update_sol(sol,p_sol);
            return 1;
        }
        return 0;
    }
    for(i=start; i<G->E; i++){
        p_sol[pos] = archi[i];
        if (comb_semplR(sol,archi,pos + 1,i +1,p_sol,p_peso + archi[i].wt,G))
            ok = 1;
    }
    return ok;
}

static int check(EDGE *p_sol,int card,EDGE *archi,GRAPH G){
    /*Verifica DAG: Un grafo è un DAG se non sono presenti archi di tipo back*/
    int i,check,time = 0,*pre,*post;
    link t,*lAdj_tmp = malloc(G->V * sizeof (link));
    pre = calloc(G->V,sizeof(int ));
    post = calloc(G->V,sizeof (int ));

    for(i= 0; i<G->V; i++)
        lAdj_tmp[i] = G->z;
    for(i=0; i<G->E; i++)
        lAdj_tmp[archi[i].u] = insertInHead(lAdj_tmp[archi[i].u],archi[i].v,archi[i].wt);
    for(i=0; i<card; i++)
        lAdj_tmp [p_sol[i].u] = removeE(lAdj_tmp [p_sol[i].u],G->z,p_sol[i].v);
    for(i=0; i< G->V; i++)
        pre[i] = post[i] = -1;

    check = dfs_isDAGr(lAdj_tmp,G->z,0,pre,post,&time);

    for(i= 0; i<G->V; i++)
        lAdj_tmp[i] = list_free(lAdj_tmp[i],G->z);
    free(lAdj_tmp);
    free(pre);
    free(post);
    return check;
}
static int dfs_isDAGr(link *lAdj, link z,int id_starter, int *pre,int *post,int *time){
    /*dfs specializzata nella individuazione di archi di tipo back*/
    int test = 1;
    link t;
    pre[id_starter] = (*time)++;
    for(t = lAdj[id_starter]; t!=z && test != 0; t = t->next) {
        if (pre[t->v] == -1)
            test = dfs_isDAGr(lAdj,z,t->v, pre, post, time); /*Seguo l'arco tree*/
        else if (post[t->v] == -1)
            return 0;/*Ho individuato un arco B, per cui non DAG, verifica fallita. Ignoro archi F e C*/
    }
    post[id_starter] = (*time)++;
    return test;
}
static void dfs_TS(GRAPH D,int id_starter,int *pre,int *time,int *ts){
    link t;
    pre[id_starter] = 1;
    for(t = D->lAdj[id_starter]; t!=D->z; t = t->next)
        if(pre[t->v] == -1)
            dfs_TS(D,t->v,pre,time,ts);
    ts[(*time)++] = id_starter;
}
static void topologic_inv(int *ts,int N){
    int i,tmp;
    for(i=0; i<N/2; i++){
        tmp = ts[N - 1 - i];
        ts[N - 1 - i] = ts[i];
        ts[i] = tmp;
    }
}
static void update_sol(comb_sol *sol,EDGE *p_sol){
    int i;
    for(i=0; i<sol->card; i++)
        sol->edgesArray[i] = p_sol[i];
}
static void getInfoSet(comb_sol *sol,GRAPH G){
    int j;
    printf("CARDINALITA min: %d\n",sol->card);
    printf("S_maxWT > \t{ ");
    for (j = 0; j < sol->card; j++)
        printf("(%s %s : %d), ", ST_GETkey(G->tab, sol->edgesArray[j].u),
               ST_GETkey(G->tab, sol->edgesArray[j].v), sol->edgesArray[j].wt);
    printf(" }");
    printf("\n");
}
GRAPH GRAPH_init(int V){
    GRAPH G = malloc(sizeof (struct graph_s));
    int i;
    G->V = V;
    G->E = 0;
    G->tab = ST_init(G->V);
    G->z = new_node(-1,0,NULL);
    G->lAdj = malloc(G->V * sizeof (link)); /*Vettore di puntatore a nodo in testa*/
    for(i=0; i<G->V; i++)
        G->lAdj[i] = G->z;
    return G;
}
void GRAPH_free(GRAPH G){
    int i;
    for(i=0; i<G->V; i++)
        G->lAdj[i] = list_free(G->lAdj[i],G->z);
    free_node(G->z);
    free(G->lAdj);
    ST_free(G->tab);
    free(G);
}
int GRAPH_GETindex(GRAPH G, ST_key chiave){
    int id = ST_GETindex(G->tab,chiave);
    if(id == -1 ){
        ST_insert(G->tab,chiave);
        id = ST_count(G->tab) - 1;
    }
    return id;
}
EDGE EDGE_create(int u,int v,int wt){
    EDGE e;
    e.u = u;
    e.v = v;
    e.wt = wt;
    return e;
}
void EDGES_extract(GRAPH G, EDGE *array){
    int i,k=0;
    link t;
    for(i=0; k < G->E; i++)
        for(t=G->lAdj[i]; t!=G->z; t= t->next)
            array[k++] = EDGE_create(i,t->v,t->wt);
}
void GRAPH_load(FILE *fin,GRAPH G){
    assert(fin != NULL);
    int i,wt_tmp;
    EDGE e;
    for(i=0; i<G->V; i++){
        fscanf(fin,"%s",l1);
        ST_insert(G->tab,l1);
    }
    while (fscanf(fin,"%s %s %d",l1,l2,&wt_tmp) == 3){
        e = EDGE_create(GRAPH_GETindex(G,l1),GRAPH_GETindex(G,l2),wt_tmp);
        G->lAdj[e.u] = insertInHead(G->lAdj[e.u],e.v,e.wt);
        G->E++;
    }
}
void GRAPH_storage(FILE *fout,GRAPH G) {
    EDGE *array = malloc(G->E * sizeof(EDGE));
    EDGES_extract(G,array);
    int i;
    fprintf(fout,"%d\n",G->V);
    for(i=0; i<G->V; i++)
        fprintf(fout,"%s\n",ST_GETkey(G->tab,i));
    for(i=0; i<G->E; i++)
        fprintf(fout,"%s %s %d\n",ST_GETkey(G->tab,array[i].u),
                ST_GETkey(G->tab,array[i].v),array[i].wt);
    free(array);
}