#include <stdio.h>
#include <stdlib.h>
#define S 20
#define SHOW_LINEAR_COMB 0
typedef struct{
    int id;
} Node;
typedef struct{
    Node node_a;
    Node node_b;
}Edge;
typedef struct{
   Node *sol;
   int num_n_sol;
}Sol;
typedef struct {
    int num_n,num_e;
    Edge *archi;
    Sol p_sol;
}tab_t;
tab_t leggiFile ();
void vertexCover(tab_t *tab);
int power_set (tab_t *tab);
int power_set_r(tab_t *tab,int k, int start, int pos);
int isVertexCover(tab_t *tab);
void free_tab(tab_t *tab);
void printSet(tab_t *tab);
int main() {
    tab_t tab = leggiFile();
    vertexCover(&tab);
    free_tab(&tab);
    return 0;
}

tab_t leggiFile (){
    char file_path[S];
    int i=0;
    FILE *fp;
    tab_t tab;
    Node a,b;
    do{
        printf("INSERIRE PERCORSO FILE VALIDO: ");
        scanf("%s",file_path);
        fp = fopen(file_path,"r");
        if(fp==NULL)printf("percorso file non valido!\n");
    }while (fp==NULL);
    if(fscanf(fp,"%d %d",&tab.num_n,&tab.num_e)!=2)
        exit(-1);
    tab.archi = (Edge*)malloc(tab.num_e*sizeof(Edge));
    while (fscanf(fp,"%d %d",&tab.archi[i].node_a.id,&tab.archi[i].node_b.id)==2)
        i = i+1;
    if(i!=tab.num_e)
        exit(-1);
    fclose(fp);
    return tab;
}

void vertexCover(tab_t *tab){
    int k = 0;
    printf("ESISTONO I SEGUENTI VERTEX COVER: \n\n");
    if(tab->num_e>=1)
        k = power_set(tab);
    if(k==0)
        printf("INSIEME_VUOTO");
}

int power_set (tab_t *tab){
    /*Evito di memorizzare le soluzioni parziali, nella ricorsione verifico se la soluzione è ammissibile. Se è ammissibile
     * incremento di 1 la cardinalità del set di output altrenativamente no. Count quindi non indica la cadinalità dell'insieme
     * delle parti ma sottoinsieme dell'insieme delle parti: insieme soluzioni valide.
     * Combinazioni semplici da k a k con con k che va da 1 a N (cardinalità insieme dei nodi)
     * Nota: la cardinalità dell'insieme delle parti di un set è pari a 2^(n=cardinalità_set)*/
    int k,count=0;
    for(k=1; k<=tab->num_n; k++){
        tab->p_sol.num_n_sol = k;
        tab->p_sol.sol = (Node *)malloc(tab->p_sol.num_n_sol*sizeof(Node));
        count += power_set_r(tab,k,0,0);
        free(tab->p_sol.sol);
    }
    return count;
}

int power_set_r(tab_t *tab,int k, int start, int pos){
    int i,count = 0;
    if(pos>=k){
        if(isVertexCover(tab)==0){
           printSet(tab);
           return 1;
        }
        else
            return 0;
    }
    for(i=start; i<tab->num_n; i++){
        tab->p_sol.sol[pos].id = i;
        count += power_set_r(tab,k,i+1,pos+1);
    }
    return count;
}

int isVertexCover(tab_t *tab){
    int i,j,k;
    short int *check =(short int *)calloc(tab->num_n,sizeof(short int));
    if(SHOW_LINEAR_COMB){
        for (int i = 0; i < tab->num_n; i++) {
            printf("%d ",check[i]);
        }
    }
    /*Il vettore check mi permette di mantenere una complessità lineare, utilizzando il metodo delle disposizioni lineari non è necessario
     * check in quanto la cardinalità di sol è sempre N ed avrà k elementi posti ad 1.
     * Ho scelto le combinazioni lineari per non compromette "l'ordine di stampa" delle soluzioni*/

    for(j=0; j<tab->p_sol.num_n_sol; j++)
        check[tab->p_sol.sol[j].id] = 1;
    for(k=0;k<tab->num_e; k++)
        if( check[tab->archi[k].node_a.id]==0 && check[tab->archi[k].node_b.id]==0 )
            break;
    free(check);
    return (tab->num_e-k);
}

void printSet(tab_t *tab){
   int i=0;
    printf("{ ");
    for (int i = 0; i < tab->p_sol.num_n_sol; i++)
        printf("%d, ",tab->p_sol.sol[i].id);
    printf("}\n\n");
}


void free_tab(tab_t *tab){
    free(tab->archi);
}
