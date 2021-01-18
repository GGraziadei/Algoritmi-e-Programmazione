#include "quotazioniBST.h"
typedef enum {c_max,c_min}count_m;
typedef struct nodo *link;
struct quotazioniBST{
    link root,z;
    int N;
    float maxQ,minQ;
    DATA dayMax,dayMin;
};
struct nodo{
    QUOTAZIONE *qDay;
    link l,r,p;
    int N;
    float maxQ,minQ;
};
static link new_node (QUOTAZIONE *qDay,link l,link r,link p,int N,float minQ,float maxQ);
static void free_node(link node_x);
static void liberaBst(link h,link z);
static link insertR(link h,QUOTAZIONE *qDay,link z);
static void BST_insert(QUOTAZIONI bst,DATA day,QUOTAZIONE *qp);
static QUOTAZIONE *searchR (link root,DATA day,link z);
static link rotR(link root);
static link rotL(link root);
static link partitionR(link root,int k);
static link balanceR(link root,link z);
static void printR(FILE *fout,link root,print_m m,link z);
static int countR(link root, link z,count_m c);
static DATA checkMaxD(link h,link z);
static DATA checkMinD(link h,link z);
static float searchBestQ(link h,link z,DATA d1, DATA d2,count_m c);
static float searchBestQUpdate(link h,link z,count_m c);

static int max(int a,int b){
   return a>=b?a:b;
}static int min(int a,int b){
    return a<=b?a:b;
}
static float maxF(float a,float b, float c){
    if (a > b){
        if(a > c) return a;
        if (c > a)return c;
    }
    if(c>b)return c;
    return b;
}static float  minF(float a,float b, float c){
    if (a < b){
        if(a < c) return a;
        if (c < a)return c;
    }
    if(c<b)return c;
    return b;
}
QUOTAZIONI BST_init(){
    QUOTAZIONI bst = malloc(sizeof (struct quotazioniBST));
    bst->z = new_node(QUOTAZIONE_void(),NULL,NULL,NULL,0,FLT_MAX,FLT_MIN);
    bst->root = bst->z;
    bst->N = 0;
    bst->minQ  = bst->maxQ = 0;
    bst->dayMax =  bst->dayMin = DAY_void();
    return bst;
}
void BST_free(QUOTAZIONI bst){
    liberaBst(bst->root,bst->z);
    bst->N = 0;
    free_node(bst->z);
    free(bst);
}
int QUOTAZIONI_add (QUOTAZIONI bst,FILE *fin,int N){
    int i;
    DATA day = DAY_void();
    QUOTAZIONE *q = NULL;
    for(i=0; i<N; i++){
        day = DAY_init(fin);
        q = searchR(bst->root,day,bst->z);
        if(q == NULL){
            q = QUOTAZIONE_init(day);
            BST_insert(bst,day,q);
        }
        QUOTAZIONE_addTRANSAZIONE(fin,q);
    }

    bst->dayMin = checkMinD(bst->root,bst->z);
    bst->dayMax = checkMaxD(bst->root,bst->z);
    bst->maxQ = searchBestQUpdate(bst->root,bst->z,c_max);
    bst->minQ = searchBestQUpdate(bst->root,bst->z,c_min);
    return N;
}
float QUOTAZIONI_max(QUOTAZIONI bst){ return bst->maxQ;}
float QUOTAZIONI_min(QUOTAZIONI bst){ return bst->minQ;}
DATA QUOTAZIONI_maxD(QUOTAZIONI bst){return bst->dayMax;}
DATA QUOTAZIONI_minD(QUOTAZIONI bst){return bst->dayMin;}
int QUOTAZIONI_qDayNum(QUOTAZIONI bst){ return bst->N;}

static DATA checkMaxD(link h,link z){
    if(h->r == z)return QUOTAZIONE_extractDAY(h->qDay);
    return checkMaxD(h->r,z);
}

static DATA checkMinD(link h,link z){
    if(h->l == z)return QUOTAZIONE_extractDAY(h->qDay);
    return checkMinD(h->l,z);
}
void QUOTAZIONI_bestQ(float  *bestMax,float *bestMin,DATA d1,DATA d2,QUOTAZIONI bst){
    (*bestMax) = searchBestQ(bst->root,bst->z,d1,d2,c_max);
    (*bestMin) = searchBestQ(bst->root,bst->z,d1,d2,c_min);
}

static float searchBestQ(link h,link z,DATA d1, DATA d2,count_m c){
    float dx,sx,todayQ;
    DATA day = QUOTAZIONE_extractDAY(h->qDay);
    if(h == z) return c==c_max?z->maxQ:z->minQ;
    todayQ = c==c_max?z->maxQ:z->minQ;
    sx = searchBestQ(h->l, z, d1, d2, c);
    if(DAY_overlap(d1,day,d2) == 1)
        todayQ = h->qDay->quotazione_day;
    dx = searchBestQ(h->r, z, d1, d2, c);
    if (c == c_max){
        return maxF(sx,dx,todayQ);
    }
    else{
        return minF(sx,dx,todayQ);
    }
}
static float searchBestQUpdate(link h,link z,count_m c){
    float dx,sx,todayQ;
    if(h == z) return c==c_max?z->maxQ:z->minQ;
    sx = searchBestQUpdate(h->l, z, c);
    todayQ = h->qDay->quotazione_day;
    dx = searchBestQUpdate(h->r, z, c);
    if (c == c_max){
        h->maxQ = maxF(sx,dx,todayQ);
        return h->maxQ;
    }
    else{
        h->minQ = minF(sx,dx,todayQ);
        return h->minQ;
    }
}

QUOTAZIONE *QUOTAZIONI_search(QUOTAZIONI bst,DATA day){
    return searchR(bst->root,day,bst->z);
}
static QUOTAZIONE *searchR (link root,DATA day,link z){
    int ris;
    if(root == z) return NULL;
    ris = DAY_cmp(day,QUOTAZIONE_extractDAY(root->qDay));
    if(ris == 0)
        return root->qDay;
    else if(ris < 0) searchR(root->l,day,z);
    else searchR(root->r,day,z);
}

static void BST_insert(QUOTAZIONI bst,DATA day,QUOTAZIONE *qp){
    bst->root = insertR(bst->root,qp,bst->z);
    bst->N = bst->root->N;
}
static link new_node (QUOTAZIONE *qDay,link l,link r,link p,int N, float min,float max){
    link node_x = malloc(sizeof (struct nodo));
    node_x->qDay = qDay;
    node_x->l = l;
    node_x->r = r;
    node_x->p = p;
    node_x->N = N;
    node_x->minQ = min;
    node_x->maxQ = max;
    return node_x;
}
static link insertR(link h,QUOTAZIONE *qDay,link z){
    int ris;
    if(h == z)
        return new_node(qDay,z,z,z,1,z->minQ,z->maxQ); /*Inserzione in foglia*/
    /*Se data presente in BST blocco la ricorsione utilizzando la proprietà di chiavi differenti*/
    ris = DAY_cmp(QUOTAZIONE_extractDAY(qDay),QUOTAZIONE_extractDAY(h->qDay) );
    if(ris < 0) {
        h->l = insertR(h->l, qDay, z);
        h->l->p = h;
    }
    else if(ris > 0) {
        h->r = insertR(h->r,qDay,z);
        h->r->p = h;
    }
    (h->N)++;
    return h;
}
static void free_node(link node_x){
    assert(node_x != NULL);
    QUOTAZIONE_free(node_x->qDay);
    free(node_x);
}
static void liberaBst(link h,link z){
    if (h == z) return;
    liberaBst(h->l,z);
    liberaBst(h->r,z);
    free_node(h);
}

static link rotR(link root){
    link x = root->l;
    root->l = x->r;
    x->r->p = root;
    x->r = root;
    x->p = root->p;
    root->p = x;
    x->N = root->N;
    root->N = 1;
    root->N += (root->l) ? root->l->N:0;
    root->N += (root->r) ? root->r->N:0;
    return x;
}
static link rotL(link root){
    link x = root->r;
    root->r = x->l;
    x->l->p = root;
    x->l = root;
    x->p = root->p;
    root->p = x;
    x->N = root->N;
    root->N = 1;
    root->N += (root->l) ? root->l->N:0;
    root->N += (root->r) ? root->r->N:0;
    return x;
}
static link partitionR(link root,int k){
    int t = root->l->N;
    if(t>k){
        /*SX > rotazione a dx */
        root->l = partitionR(root->l,k);
        root = rotR(root);
    }
    else if(t<k){
        /*DX > rotazione a sx */
        root->r = partitionR(root->r,k-t-1);
        root = rotL(root);
    }
    return root;
}

void QUOTAZIONI_balance(QUOTAZIONI bst){
    int cMax,cMin;
    cMax = countR(bst->root,bst->z,c_max);
    cMin = countR(bst->root,bst->z,c_min);
    printf("\tCAMMINO MAX: %d, CAMMINO MINIMO: %d\n",cMax,cMin);
    if(cMax / cMin > S_soglia_bilanciamento){
        bst->root = balanceR(bst->root,bst->z);
        cMax = countR(bst->root,bst->z,c_max);
        cMin = countR(bst->root,bst->z,c_min);
        printf("Bilanciamento completato\n\tCAMMINO MAX: %d, CAMMINO MINIMO: %d\n",cMax,cMin);
    }

    bst->maxQ = searchBestQUpdate(bst->root,bst->z,c_max);
    bst->minQ = searchBestQUpdate(bst->root,bst->z,c_min);
}
static link balanceR(link root,link z){
    int r;
    if(root == z)
        return root;
    r = (root->N + 1)/2 -1;
    root = partitionR(root,r);
    /*Bilanciamento albero dx e sx*/
    root->l = balanceR(root->l,z);
    root->r = balanceR(root->r,z);
    return root;
}
static int countR(link root, link z,count_m c){
    if(root == z) return 0;
    if(c == c_max)
        return 1+max(countR(root->l,z,c),countR(root->r,z,c));
    else if(c == c_min)
        return 1+min(countR(root->l,z,c),countR(root->r,z,c));
}

void BST_storage(FILE *fout,QUOTAZIONI  bst){
    printf("\nSeleziona strategia di stampa: \n");
    char *selectStrqategy[none] = {
        "INORDER",
        "POSTORDER",
        "PREORDER",
    };
    print_m sel;
    for (sel = inorder; sel < none; sel++)
        printf("%d > %s\n",sel,selectStrqategy[sel]);
    scanf("%d",&sel);
    printR(fout,bst->root,sel,bst->z);
}
static void printR(FILE *fout,link root,print_m m,link z){
    if(root == z)return;
    if(m == preorder)
        QUOTAZIONE_print(fout,root->qDay);
    printR(fout,root->l,m,z);
    if(m == inorder)
        QUOTAZIONE_print(fout,root->qDay);
    printR(fout,root->r,m,z);
    if(m == postorder)
        QUOTAZIONE_print(fout,root->qDay);
}

