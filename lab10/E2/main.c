#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define S 50+1
#define GDB 0
#define NO_MEMOIZED -100
typedef enum tipo{
    e_zaffiro,e_rubino,e_topazio,e_smeraldo,e_void
} e_tipo;
typedef struct{
    e_tipo type;
    int num;
}Item;
typedef struct{
    Item *pietre;
    int sol;
    int n;
}tab_t;

tab_t init_set (FILE *);
void free_tab(tab_t *DB);
int openFile (FILE **fp);
void maxL(tab_t *tab);
int *****init_memoMatrix(tab_t *tab);
void free_memoMatrix(tab_t *tab,int *****m);
int max(int a,int b);
int fT(int *****memoMatrix,int z,int r,int t,int s);
int fS(int *****memoMatrix,int z,int r,int t,int s);
int fZ(int *****memoMatrix,int z,int r,int t,int s);
int fR(int *****memoMatrix,int z,int r,int t,int s);

int main() {
    setbuf(stdout, 0);
    FILE *fp;
    int i,test_num;
    test_num = openFile(&fp);
    for (int i = 0; i < test_num; i++) {
        printf("\n\nTEST #%d\n",i+1);
        tab_t DB = init_set(fp);
        maxL(&DB);
        printf("\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d\nMAX_L %d\n",
               DB.pietre[e_zaffiro].num,DB.pietre[e_rubino].num,DB.pietre[e_topazio].num,DB.pietre[e_smeraldo].num,DB.sol);
        free_tab(&DB);
    }
    fclose(fp);
    return 0;
}

void  maxL(tab_t *tab){
    int z,r,t,s,*****memoMatrix,tmp;
    z = tab->pietre[e_zaffiro].num;
    r = tab->pietre[e_rubino].num;
    t = tab->pietre[e_topazio].num;
    s = tab->pietre[e_smeraldo].num;
    memoMatrix = init_memoMatrix (tab);
    /*Si mostra che possono esistere più soluzioni di pari lunghezza (differente pietra starter),
     * utile a spiegare i risultati ottenuti nel precedente lab*/
    tmp = fZ(memoMatrix,z,r,t,s);
    printf("CON ZAFFIRO starter: %d\n",tmp);
    tab->sol = max(tab->sol,tmp);
    tmp = fR(memoMatrix,z,r,t,s);
    printf("CON RUBINO starter: %d\n",tmp);
    tab->sol = max(tab->sol ,tmp );
     tmp = fT(memoMatrix,z,r,t,s);
    printf("CON TOPAZIO starter: %d\n",tmp);
    tab->sol = max(tab->sol , tmp);
    tmp = fS(memoMatrix,z,r,t,s);
    printf("CON SMERALDO starter: %d\n",tmp);
    tab->sol = max(tab->sol , tmp);
    free_memoMatrix(tab,memoMatrix);
}
int fZ(int *****memoMatrix,int z,int r,int t,int s){
    if(memoMatrix[e_zaffiro][z][r][t][s] != NO_MEMOIZED)
        return memoMatrix[e_zaffiro][z][r][t][s];
    if(z <= 0) return 0;
    memoMatrix[e_zaffiro][z][r][t][s] = 1+max(fR(memoMatrix,z-1,r,t,s),fZ(memoMatrix,z-1,r,t,s));
    return memoMatrix[e_zaffiro][z][r][t][s];
}
int fR(int *****memoMatrix,int z,int r,int t,int s){
    if(memoMatrix[e_rubino][z][r][t][s] != NO_MEMOIZED)
        return memoMatrix[e_rubino][z][r][t][s];
    if(r <= 0) return 0;
    memoMatrix[e_rubino][z][r][t][s] = 1+max(fT(memoMatrix,z,r-1,t,s),fS(memoMatrix,z,r-1,t,s));
    return memoMatrix[e_rubino][z][r][t][s];
}
int fS(int *****memoMatrix,int z,int r,int t,int s){
    if(memoMatrix[e_smeraldo][z][r][t][s] != NO_MEMOIZED)
        return memoMatrix[e_smeraldo][z][r][t][s];
    if(s <= 0) return 0;
    memoMatrix[e_smeraldo][z][r][t][s] = 1+max(fT(memoMatrix,z,r,t,s-1),fS(memoMatrix,z,r,t,s-1));
    return memoMatrix[e_smeraldo][z][r][t][s];
}
int fT(int *****memoMatrix,int z,int r,int t,int s){
    if(memoMatrix[e_topazio][z][r][t][s] != NO_MEMOIZED)
        return memoMatrix[e_topazio][z][r][t][s];
    if(t <= 0) return 0;
    memoMatrix[e_topazio][z][r][t][s] = 1+max(fR(memoMatrix,z,r,t-1,s),fZ(memoMatrix,z,r,t-1,s));
    return memoMatrix[e_topazio][z][r][t][s];
}

int *****init_memoMatrix(tab_t *tab){
    int z,r,t,s,*****m;
    e_tipo pietra_starter;
    m = (int *****) malloc( tab->n*sizeof (int ****));
    assert(m != NULL);
    for(pietra_starter = e_zaffiro; pietra_starter<tab->n; pietra_starter++) {
        m[pietra_starter] = (int ****) malloc((tab->pietre[e_zaffiro].num + 1)*sizeof (int ***));
        assert(m[pietra_starter] != NULL);
        for (z = 0; z <= tab->pietre[e_zaffiro].num; z++) {
            m[pietra_starter][z] = (int ***) malloc((tab->pietre[e_rubino].num + 1) * sizeof(int **));
            assert(m[pietra_starter][z] != NULL);
            for (r = 0; r <= tab->pietre[e_rubino].num; r++) {
                m[pietra_starter][z][r] = (int **) malloc((tab->pietre[e_topazio].num + 1) * sizeof(int *));
                assert(m[pietra_starter][z][r] != NULL);
                for (t = 0; t <= tab->pietre[e_topazio].num; t++) {
                    m[pietra_starter][z][r][t] = (int *) malloc((tab->pietre[e_smeraldo].num + 1) * sizeof(int));
                    assert(m[pietra_starter][z][r][t] != NULL);
                    for (s = 0; s <= tab->pietre[e_smeraldo].num; s++) {
                        m[pietra_starter][z][r][t][s] = NO_MEMOIZED;
                    }
                }
            }
        }
    }
    return m;
}

void free_memoMatrix(tab_t *tab,int *****m){
    int z,r,t;
    e_tipo pietra_starter;
    for(pietra_starter = e_zaffiro; pietra_starter<tab->n; pietra_starter++) {
        for (z = 0; z <= tab->pietre[e_zaffiro].num; z++) {
            for (r = 0; r <= tab->pietre[e_rubino].num; r++) {
                for (t = 0; t <= tab->pietre[e_topazio].num; t++) {
                    free(m[pietra_starter][z][r][t]);
                }
                free(m[pietra_starter][z][r]);
            }
            free(m[pietra_starter][z]);
        }
        free(m[pietra_starter]);
    }
    free(m);
}

tab_t init_set (FILE *fp){
    tab_t tab_t;
    int j;
    tab_t.pietre = malloc(e_void*sizeof(Item));
    tab_t.n = e_void;
    tab_t.sol = NO_MEMOIZED;
    for (int j = 0; j < e_void; j++) {
        fscanf(fp,"%d",&tab_t.pietre[j].num);
        tab_t.pietre[j].type = j;
    }
    return tab_t;
}

void free_tab(tab_t *DB){
    free(DB->pietre);
}
int max(int a, int b){
    if(a>=b)return a;
    else return b;
}
int openFile (FILE **fp){
    int i;
    char str[S];
    do{
        printf("Inserire percorso file: ");
        gets(str);
        (*fp) = fopen(str,"r");
        if((*fp) == NULL)
            printf("Percorso file non valido!\n");
    }while ((*fp) == NULL);
    fscanf((*fp),"%d",&i);
    return i;
}