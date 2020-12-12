#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define S 50+1
#define GDB 1
typedef int time;
typedef struct{
    time inizio,fine,l;
} att;
typedef struct tabAtt *linkTabAtt;
struct tabAtt{
    int N;
    att *v;
};
linkTabAtt init_tabAtt();
linkTabAtt liberaTabAtt (linkTabAtt tabAtt);
int init_att(att **a);
void ordinaAtt(linkTabAtt tab);
void MergeSortR (linkTabAtt tab, int inf,int sup,att *v_tmp);
void Merge(linkTabAtt tab,int inf, int q, int sup, att *v_tmp);
void stampaAtt(att *v, int N);
void stampaAttI(att x, int index);
void attSel (int N, att *v);
int binarySearch(att *v, int inf,int sup, int i);
void stampaSolR(att *v,int *P,int last);
int max(int a,int b);
static char buf[S];
int main() {
    setbuf(stdout, 0);
    linkTabAtt tab = init_tabAtt();
    ordinaAtt(tab);
    attSel (tab->N , tab->v);
    tab = liberaTabAtt(tab);
    return 0;
}
/*Soluzione ottima calcolata LAB9->1
att1.txt CARDINALITA' 2 - VALORE 10
S={ (0, 6), (6, 10), }.*/
void attSel (int N, att *v){
    int i,j,ris,last;
    int L[N],P[N];

    last = 1;
    ris = L[0] = v[0].l;
    P[0] = -1;

    for(i=1; i<N; i++){
        L[i] = v[i].l;
        P[i] = -1;

        if(ris > L[i-1])
            j = last;
        else
            j = i-1;
        j = binarySearch(v,0,i-1,i);
        if (j >= 0 && L[i] < L[j] + v[i].l) {
            L[i] += L[j];
            P[i] = j;
            #if GDB
                printf("\nI: %d L:%d P:%d\n",i,L[i],P[i]);
            #endif
        }

        if(ris < L[i]){
            ris = L[i];
            last = i;
        }
    }
    /*Mi aspetto al più di 1 attività*/
    printf("\n\nUNA DELLE SEQUENZE DI ATTIVITA OTTIME: (VALUE %d)\n",ris);
    stampaSolR(v,P,last);
}

int binarySearch(att *v, int inf,int sup, int i){
    int q;
    if(inf>=sup)
        if(v[inf].fine <= v[i].inizio)
            return inf;
        else
            return 0;
    q = (inf + sup)/2;
    if (v[q].fine <= v[i].inizio)
        return  max (q,binarySearch(v,q+1,sup, i));
    else
        return  binarySearch(v,inf,q, i);
}
void stampaSolR(att *v,int *P,int last){
    if(P[last] == -1) {
        stampaAttI(v[last], last);
        return;
    }
    stampaSolR(v,P,P[last]);
    stampaAttI(v[last], last);
}
linkTabAtt init_tabAtt(){
    linkTabAtt tab = malloc(sizeof (struct tabAtt));
    tab->N = init_att(&(tab->v));
    return tab;
}
linkTabAtt liberaTabAtt (linkTabAtt tabAtt){
    free(tabAtt->v);
    free(tabAtt);
    return NULL;
}
int init_att(att **a){
    int N=0,i;
    FILE *in = NULL;
    do {
        printf("Inserire percorso file valido: ");
        scanf("%s",buf);
        in = fopen(buf,"r");
        if(in == NULL)printf("Percorso file non valido");
    }while (in == NULL);
    fscanf(in,"%d",&N);
    N = N+1;
    (*a) = malloc(N*sizeof(att));
    for(i=0; i<N-1; i++) {
        fscanf(in, "%d %d", &(*a)[i].inizio, &(*a)[i].fine);
        (*a)[i].l = (*a)[i].fine - (*a)[i].inizio;
    }
    (*a)[N-1].l = (*a)[N-1].fine =  (*a)[N-1].inizio = 0;
    return N;
}
 void ordinaAtt (linkTabAtt tab){
    att *v_tmp = malloc(sizeof(att)*tab->N);
    MergeSortR(tab,0,tab->N-1,v_tmp);
    free(v_tmp);
    printf("SET ATTIVITA:\n");
    stampaAtt(tab->v,tab->N);
}

void MergeSortR (linkTabAtt tab, int inf,int sup, att *v_tmp){
    if(inf >= sup)
        return;
    int q = (inf + sup)/2;
    MergeSortR(tab,inf,q,v_tmp);
    MergeSortR(tab,q+1,sup,v_tmp);
    Merge(tab,inf,q,sup,v_tmp);
}

void Merge(linkTabAtt tab,int inf, int q, int sup, att *v_tmp){
    #if GDB
    printf("%d %d\n",inf,sup);
    #endif
    int i,j,k;
    i = inf;
    j = q+1;
    for(k=inf; k<=sup; k++){
        if(i > q)
            v_tmp[k]=tab->v[j++];
        else if(j > sup)
            v_tmp[k] = tab->v[i++];
        else if (tab->v[i].fine < tab->v[j].fine )
            v_tmp[k] = tab->v[i++];
         else if (tab->v[i].fine == tab->v[j].fine && tab->v[i].inizio > tab->v[j].inizio)
                v_tmp[k] = tab->v[i++];
        else
            v_tmp[k]=tab->v[j++];
    }
    for(k=inf; k<=sup; k++)
        tab->v[k] = v_tmp[k];
}
void stampaAtt(att *v, int N){
    int i;
    for (i = 0; i<N; i++)
        stampaAttI(v[i],i);
}
void stampaAttI(att x, int index){
    printf("%d # INIZIO %d FINE %d -- durata %d\n",index,x.inizio,x.fine,x.l);
}
int max(int a,int b){
    if(a>b)return a;
    else return b;
}