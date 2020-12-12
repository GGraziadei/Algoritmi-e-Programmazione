#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define SCACCHIERA "board.txt"
#define TESSERE "tiles.txt"

#define GDB 0

typedef struct{
    int check;
    int value[2];
    char color[2];
}tessera;

typedef struct {
    int id_tiles,r; /*ID tail e rotazione tail*/
}tiles_in_board;

typedef struct{
    int R,C,classe,value_now;
    tiles_in_board **mat;
}scacchiera;

typedef struct {
    scacchiera *board;
    tessera *tiles;
    tiles_in_board **best_sol;
    int num_tiles,best_value;
}*tab;

tab init_tab();
tessera *init_tiles(tab t);
scacchiera *init_board (tab t);
tiles_in_board **init_sol(int R, int C);
int countValue(tab t);
void loadBoard(scacchiera *s,tessera *tiles,FILE *in);
void loadTiles (tessera *tiles, FILE *in, int N);
void disp_sempl(tab t);
void disp_semplR(tab t, int pos);
void updateBest(tab t);
void stampaMat (FILE *out,int R,int C,tiles_in_board **sol);
void stampaValue (FILE *out,int R,int C,int *righe,int *colonne);
void free_tab(tab t);

int main() {
    tab t = init_tab();
    t->best_value = t->board->value_now  = 0;
    disp_sempl(t);
    free_tab(t);
    return 0;
}
void disp_sempl(tab t){
    printf("CONFIGURAZIONE INIZIALE\nVALORE: %d\n",countValue(t));
    stampaMat(stdout,t->board->R,t->board->C,t->board->mat);
    disp_semplR(t,0);
    printf("\n\nCONFIGURAZIONE FINALE\nVALORE: %d\n",t->best_value);
    stampaMat(stdout,t->board->R,t->board->C,t->best_sol);
}
void disp_semplR(tab t, int pos){
    int i,j,k;
    if(pos >= t->board->classe){
    /*Condizione di terminazione: disposizione di una tessera per ogni cella*/
        t->board->value_now = countValue(t);
        if(t->board->value_now > t->best_value)
            updateBest(t);
        return;
    }
    i = pos / t->board->R;
    j = pos % t->board->C;
    /*Se pos già riempito ricorro su pos+1*/
    if(t->board->mat[i][j].id_tiles != -1){
        disp_semplR(t,pos+1);
        return;
    }
    for ( k = 0; k < t->num_tiles; k++) {
        if(t->tiles[k].check == 0 ){
            t->tiles[k].check = 1;
            t->board->mat[i][j].id_tiles = k;
            /*Ricorro per le due possibili soluzioni del rotore*/
            t->board->mat[i][j].r = 0;
            disp_semplR(t,pos+1);
            t->board->mat[i][j].r = 1;
            disp_semplR(t,pos+1);
            /*Libero la tessera dalla scacchiera e predispongo la cella per ospitare una nuova tessera*/
            t->board->mat[i][j].id_tiles = -1;
            t->tiles[k].check = 0;
        }
    }
}


int countValue(tab t){
    /*countValue conta il valore di una scacchiera dopo aver passato la checkSol */
    int i,j,idA,idB,rA,rB,value = 0;
    char colorA,colorB;
    int *righe = (int *)calloc(t->board->R,sizeof(int));
    int *colonne = (int *)calloc(t->board->C,sizeof(int));
    for(i=0; i<t->board->R; i++){
        idA = t->board->mat[i][0].id_tiles;
        rA = t->board->mat[i][0].r;
        colorA = t->tiles[idA].color[rA];
        righe[i] = t->tiles[idA].value[rA];
        for ( j = 1; j <t->board->C && righe[i]!=0; j++) {
            idB = t->board->mat[i][j].id_tiles;
            rB = t->board->mat[i][j].r;
            colorB = t->tiles[idB].color[rB];
            value  = t->tiles[idB].value[rB];
            if(colorA == colorB)
                righe[i] += value;
            else righe[i] = 0;
            idA = idB;
            rA = rB;
            colorA  =colorB;
        }
    }
    for (j = 0; j < t->board->C; j++) {
        idA = t->board->mat[0][j].id_tiles;
        rA =  1-t->board->mat[0][j].r;
        colorA = t->tiles[idA].color[rA];
        colonne[j] = t->tiles[idA].value[rA];
        for ( i = 1; i < t->board->R && colonne[j]!=0; i++) {
            idB = t->board->mat[i][j].id_tiles;
            rB = 1- t->board->mat[i][j].r;
            colorB = t->tiles[idB].color[rB];
            value = t->tiles[idB].value[rB];
            if (colorA == colorB)
                colonne[j] += value;
            else
                colonne[j] = 0;
            idA = idB;
            rA = rB;
            colorA  =colorB;
        }
    }
    value = 0;
    for (i = 0; i < t->board->R; i++)
        value += righe[i];
    for ( i = 0; i < t->board->C; i++)
        value += colonne[i];
#if GDB
    printf("\n\n____________________VALORE %d\n",value);
    stampaMat(stdout,t->board->R,t->board->C,t->board->mat);
    stampaValue(stdout,t->board->R,t->board->C,righe,colonne);
#endif
    free(righe);
    free(colonne);
    return value;
}
void updateBest(tab t){
    int i,j;
    t->best_value = t->board->value_now;
    for (i=0; i<t->board->R; i++)
        for (j=0; j<t->board->C; j++)
            t->best_sol[i][j]= t->board->mat[i][j];
}
void stampaMat (FILE *out,int R,int C,tiles_in_board **sol){
    int i,j;
    for(i=0; i<R; i++){
        for(j=0; j<C; j++)
            fprintf(out," %d/%d  ",sol[i][j].id_tiles,sol[i][j].r);
        fprintf(out,"\n");
    }
}
void stampaValue (FILE *out,int R,int C,int *righe,int *colonne){
    int i;
    fprintf(out,"\nRIGHE\n");
    for(i=0; i<R; i++)
        fprintf(out," %d  \n",righe[i]);
    fprintf(out,"\nCOLONNE\n");
    for(i=0; i<C; i++)
        fprintf(out," %d  ",colonne[i]);
}
tab init_tab(){
    tab t;
    t = malloc(sizeof(*t));
    assert(t != NULL);
    t->tiles = init_tiles(t);
    t->board = init_board(t);
    t->best_sol  = init_sol(t->board->R,t->board->C);
    return t;
}

tessera *init_tiles(tab t){
    tessera *tiles;
    FILE *in = fopen(TESSERE,"r");
    fscanf(in,"%d",&t->num_tiles);
    tiles = malloc(t->num_tiles*sizeof(tessera) );
    assert(tiles != NULL);
    loadTiles(tiles,in,t->num_tiles);
    fclose(in);
    return tiles;
}
void loadTiles (tessera *tiles, FILE *in, int N){
    int i;
    for (int i = 0; i < N; i++){
        tiles[i].check = 0;
        fscanf(in," %c %d %c %d",&tiles[i].color[0],
               &tiles[i].value[0],&tiles[i].color[1],&tiles[i].value[1]);
    }
}

scacchiera *init_board (tab t){
    scacchiera *s = malloc(sizeof(scacchiera));
    FILE *in = fopen(SCACCHIERA,"r");
    fscanf(in,"%d %d",&s->R,&s->C);
    s->classe = (s->C) * (s->R);
    s-> mat = init_sol(s->R,s->C);
    loadBoard(s,t->tiles,in);
    s->value_now = 0;
    fclose(in);
    return s;
}
void loadBoard(scacchiera *s,tessera *tiles,FILE *in){
    for (int i = 0; i < s->R; i++){
        for (int j = 0; j < s->C; j++) {
            fscanf(in,"%d/%d",&s->mat[i][j].id_tiles,&s->mat[i][j].r);
            if(s->mat[i][j].id_tiles != -1)
                tiles[s->mat[i][j].id_tiles].check = 1;
        }
    }
}

tiles_in_board **init_sol(int R, int C){
    int i,j;
    tiles_in_board **sol = (tiles_in_board**) malloc(R*sizeof(tiles_in_board*));
    assert(sol != NULL);
    for (i=0; i<R; i++){
        sol[i] = (tiles_in_board *)malloc(C*sizeof(tiles_in_board));
    }
    return sol;
}

void free_tab(tab t){
    int i;
    for (int i = 0; i < t->board->R; i++)
        free(t->board->mat[i]);
    for (int i = 0; i < t->board->R; i++)
        free(t->best_sol[i]);
    free(t->tiles);
    free(t->board->mat);
    free(t->board);
    free(t->best_sol);
    free(t);
}