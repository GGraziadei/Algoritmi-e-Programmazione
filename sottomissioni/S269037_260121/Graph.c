#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Graph.h"
#define DBG 0
typedef struct node *link;
struct node { int v; int wt; link next; } ;
struct graph_s { int V; int E; link *ladj; stanza *s; link z; } ;
struct path_s{link h,tail; int N; float value;};
static Edge  EDGEcreate(int v, int w, int wt);
static link  NEW(int v, int wt, link next);
static void LISTinshead(PATH p,int v,int wt);
static void svuotaLISTA(PATH p);
static void  insertE(Graph G, Edge e);
static PATH GRAPHpathBestR(Graph g, int M, int PF, int id_start,int id_now,PATH p,PATH bestP,int tesoroStatus );
static PATH PATHinit();
static void PATHfree(PATH p);
void printPATH(PATH p,Graph g);
static int LISTextractP(link *hp);
static int GRAPHpathCheck(Graph g, PATH p,int M,int PF);

void GRAPHpathBest(Graph g, int M, int PF) {
    PATH p,bestP;
    p  = PATHinit();
    bestP = PATHinit();
    bestP->value = -1; /*Parto con una soluzione non valida*/
    p->h = bestP->h = NULL;
    bestP = GRAPHpathBestR(g,M,PF,0,0,p,bestP,1);
    if(bestP->value == -1) printf("fallimento!");
    else {printf("M %d PF %d ",M,PF); printPATH(bestP,g);}
    PATHfree(p);
    PATHfree(bestP);
}
static PATH GRAPHpathBestR(Graph g, int M, int PF, int id_start,int id_now,PATH p,PATH bestP,int tesoroStatus) {
    int tesoroNOW = 0;
    float value,tmp;
    link t;
    if((id_start == id_now && p->N != 0) || M == 0 || PF == 0){
        tmp = p->value;
        if(id_start != id_now){
          if (g->s[id_now].p == 1)
            tmp= p->value - (p->value / 3);
          if (g->s[id_now].p == 2)
            tmp = p->value - (p->value / 2);
          else if (g->s[id_now].p >= 3)return bestP;
        }
        if(bestP->value < tmp){
            bestP->value = tmp;
            bestP->N = p->N;
            t = p->h;
            svuotaLISTA(bestP);
            while(t!=NULL){
                LISTinshead(bestP,t->v,t->wt);
                t = t->next;
            }
        }
        #if DBG
        printPATH(bestP,g);
        #endif
        return bestP;
    }

    value = g->s[id_now].countP == 0 ? g->s[id_now].o:0;
    g->s[id_now].countP ++;
    p->N ++;

    if(tesoroStatus == 1 && g->s[id_now].v > 0){
        tesoroStatus = 0;
        tesoroNOW = 1;
        value += g->s[id_now].v;
        tesoroStatus -= g->s[id_now].v;
    }
    if(tesoroNOW == 0 && abs(tesoroStatus) < g->s[id_now].v){
        value +=  (tesoroStatus +  g->s[id_now].v);
        tesoroStatus = g->s[id_now].v * (-1);
    }
    LISTinshead(p,id_now,value);
    p->value += value;
    for(t=g->ladj[id_now]; t!=g->z; t = t->next)
        bestP = GRAPHpathBestR(g,M - 1,PF + t->wt,id_start,t->v,p,bestP,tesoroStatus);
    p->value -= value;
    p->N--;
    g->s[id_now].countP--;
    LISTextractP(&p->h);
    return bestP;
}
PATH GRAPHpathLoad(Graph g, FILE *fp,int M,int PF) {
    PATH test = PATHinit();
    link t;
    char buf1[20+1];
    while(fscanf(fp,"%s",buf1)==1)
        LISTinshead(test,GRAPHgetIndex(g,buf1),0);
    if(test->tail->v == 0 && GRAPHpathCheck(g,test,M,PF) == 1 ){
        printf("PATH proposto valido\n");
    }else printf("PATH proposto non valido !\n");
    return test;
}
static int GRAPHpathCheck(Graph g, PATH p,int M,int PF) {
    link t,pre,tt; /*Guardo l'arco che congiunge due stanze e sommo il peso dell'arco che può essere 0 oppure -1*/
    for(t=p->h->next,pre = p->h; t!=NULL;pre=t, t=t->next){
        M -= 1;
        for(tt = g->ladj[t->v]; tt!= g->z; tt=tt->next)
            if(tt->v == pre->v){
                PF+= tt->wt;
                break;
            }
        if(tt == g->z)
            return 0; /*Nessun arco tra stanza pre->v e stanza t->v*/

        p->value += g->s[t->v].o;
        if(M == 0 && t->v != 0)return 0;
        if(PF == 0 && p->value == 0)return 0;
        if(PF == 0 && p->value ){
            if(g->s[t->v].p == 1)p->value -= (p->value / 3);
            else if(g->s[t->v].p == 2) p->value -= (p->value / 2);
            else return 0;
        }
    }
    return 1;
}

static Edge EDGEcreate(int v, int w, int wt) {
  Edge e;
  e.v = v;
  e.w = w;
  e.wt = wt;
  return e;
}

static link NEW(int v, int wt, link next) {
  link x = malloc(sizeof *x);
  if (x == NULL)
    return NULL;
  x->v = v;
  x->wt = wt;
  x->next = next;
  return x;
}

Graph GRAPHinit(int N) {
  int v;
  Graph G = malloc(sizeof *G);
  if (G == NULL) return NULL;
  G->V = N;
  G->E = 0;
  G->z = NEW(-1, 0, NULL);
  if (G->z == NULL) return NULL;
  G->ladj = malloc(N *sizeof(link));
  if (G->ladj == NULL) return NULL;
  for (v = 0; v < G->V; v++)
    G->ladj[v] = G->z;
  G->s = malloc(N * sizeof (stanza));
  if (G->s == NULL) return NULL;
  return G;
}

void GRAPHfree(Graph G) {
  int v;
  link t, next;
  for (v=0; v < G->V; v++)
    for (t=G->ladj[v]; t != G->z; t = next) {
      next = t->next;
      free(t);
    }
  free(G->s);
  free(G->ladj);
  free(G->z);
  free(G);
}
stanza STANZAcrea(char *buf,int p,int v,int wt){
    stanza s;
    strcpy(s.nome,buf);
    s.p = p;
    s.o = wt;
    s.v = v;
    s.countP = 0;
    return s;
}
int GRAPHgetIndex(Graph g,char *nome){
    int i;
    for(i=0; i<g->V && strcmp(g->s[i].nome,nome) != 0; i++);
    if(i == g->V)return -1;
    return i;
}
Graph GRAPHload(FILE *fp) {
  int N,v,id1, id2,p,wt;
  char label1[S], label2[S];
  fscanf(fp, "%d", &N);
  Graph g = GRAPHinit(N);
  N = 0;
  while(N < g->V){
      fscanf(fp,"%s %d %d %d",label1,&p,&v,&wt);
        if(GRAPHgetIndex(g,label1) == -1)
            g->s[N++] = STANZAcrea(label1,p,v,wt);
  }
  while (fscanf(fp,"%s %s %d",label1,label2,&wt) == 3){
      id1 = GRAPHgetIndex(g,label1);
      id2 = GRAPHgetIndex(g,label2);
      if(wt == 1)wt = -1;
      GRAPHinsertE(g, id1, id2, wt);
  }
  return g;
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
  insertE(G, EDGEcreate(id1, id2, wt));
}

void GRAPHstore(Graph G, FILE *fout) {
  int i;
  Edge *a;
  a = malloc(G->E * sizeof(Edge));
  if (a == NULL) return;
  GRAPHedges(G, a);
  fprintf(fout, "%d\n", G->V);
  for (i = 0; i < G->V; i++)
   printSTANZA(&G->s[i]);
  for (i = 0; i < G->E; i++)
    fprintf(fout, "%s  %s %d\n",G->s[a[i].v].nome, G->s[a[i].w].nome, a[i].wt);
}

static void  insertE(Graph G, Edge e) {
  int v = e.v, w = e.w, wt = e.wt;
  G->ladj[v] = NEW(w, wt, G->ladj[v]);
  G->ladj[w] = NEW(v, wt, G->ladj[w]);
  G->E++;
}

void  GRAPHedges(Graph G, Edge *a) {
  int v, E = 0;
  link t;
  for (v=0; v < G->V; v++)
    for (t=G->ladj[v]; t != G->z; t = t->next)
      if (v < t->v)
        a[E++] = EDGEcreate(v, t->v, t->wt);
}
void printPATH(PATH p,Graph g){
    link t;
    printf("VALORE: %.02f LEN: %d\n\n%15s %20s %30s\n",p->value,p->N,"VALUE_PASSAGGIO","NOME","PREMI ALLO START");
    for(t=p->h; t!= NULL; t = t->next) {
        printf("\t %d \t",t->wt);
        printSTANZA(&g->s[t->v]);
    }
}
void printSTANZA(stanza *s){
    printf("%20s [p: %d] \t oro_start: %d \t tesoro_start: %d\n",s->nome,s->p,s->o,s->v);
}
static PATH PATHinit(){
    PATH p = malloc(sizeof(struct path_s));
    p->h = p->tail =  NULL;
    p->N = p->value = 0;
    return p;
}
static link svuota(link h){
    link t,v;
    for(t=h; t!=NULL; ){
        v = t->next;
        free(t);
        t = v;
    }
    return NULL;
}
static void PATHfree(PATH p){
    p->h = svuota(p->h);
    p->N = p->value = 0;
}
static void svuotaLISTA(PATH p){
    p->h = svuota(p->h);
}
static void LISTinshead(PATH p,int v,int wt){
    p->h = NEW(v,wt,p->h);
    if(p->tail == NULL)p->tail = p->h;
}
static int LISTextractP(link *hp){
    link t = *hp;
    int s_id;
    if(t == NULL)return -1;
    s_id  = t->v;
    *hp = t->next;
    free(t);
    return s_id;
}