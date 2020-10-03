#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define S0 20+1
#define S1 100+1
#define N 30

// posso cambiare vocabolario
#define RIGA "riga"
#define COLONNA "colonna"
#define FINE "fine"
#define DESTRA "destra"
#define SINISTRA "sinistra"
#define SU "su"
#define GIU "giu"

typedef struct {
int s; //0 RIGA -- 1 COLONNA
int x; //indice riga o colonna
int dir;//1 DESTRA o GIU -- -1 SINISTRA o SU
int p;
}Mossa;

typedef struct {
    int r;
    int c;
    int m[N][N];
}Info;

void toMinuscolo (char*, int );
int creaMossa (Mossa*, Info );
Info creaMatrix();
void stampaMatrix(Info );
int semplifica(int,int);
void eseguiMossa (Mossa , Info*);
void stampaMatrixFile(Info);

int main()
{
    Info matrix = creaMatrix();
    Mossa mossa;
    stampaMatrix(matrix);

        while( creaMossa(&mossa,matrix)){
            eseguiMossa(mossa,&matrix);
            stampaMatrix(matrix);
        }

    stampaMatrixFile(matrix);


    return 0;
}


int creaMossa (Mossa *mossa, Info matrix){
    char str[S1], selettore[S0];
    char direzione[S0];
    int indice,posizioni,corretto=0;

    do{
        fflush(stdin);
        corretto = 0;

        printf("Inserire comando \n<selettore> <indice> <direzione> <posizioni>: ");
        gets(str);

        int na = sscanf(str,"%s %d %s %d",selettore, &indice, direzione, &posizioni);

        toMinuscolo(selettore,strlen(selettore));
        toMinuscolo(direzione,strlen(direzione));

        if( na != 4 ) {//Verifica formato input
          if(!strcmp(FINE,selettore)){ //verifica uscita
                printf("FINE ESECUZIONE");
                return 0;
          }
          else printf("verifica formato input non superata\n");
        }
        // verifica logica input
        else if
                 ( ( !strcmp(RIGA,selettore) && (!strcmp(DESTRA,direzione) || !strcmp(SINISTRA,direzione) ) )||
                 (!strcmp(COLONNA,selettore) && (!strcmp(SU,direzione) || !strcmp(GIU,direzione))) )

        {

            corretto = 1;
            if(!strcmp(COLONNA,selettore)) mossa->s = 1;
            else  mossa->s = 0;
            mossa->p = posizioni;
            mossa->x = indice - 1; //L'utente non pensa come il c
            if(!strcmp(DESTRA,direzione) || !strcmp(GIU,direzione)) mossa->dir = 1;
            else mossa->dir = -1;

            if((mossa->dir && indice>matrix.r) || (!mossa->dir && indice>matrix.c) ){
                printf("verifica logica input non superata\n");
                corretto = 0;
            }
            return corretto;


        }
        else printf("verifica logica input non superata\n");
    } while(!corretto);

return 0;
}

void toMinuscolo (char str[], int l){
 int i;
 for(i=0; i<l; i++)
    str[i] = tolower(str[i]);
return;
}

Info creaMatrix(){
    Info m;
    int i,j,corretto =0;
    char f_name[S0] ;
    FILE *fp;

        do{
            printf("Inserire nome file: ");
            gets(f_name);
                if(f_name != NULL ){
                    fp=fopen(f_name,"r");
                        if(fp!=NULL) corretto =1;
                        //else printf("VUOTO\n");
                }
                else printf("file non trovato\n");
        }while(!corretto);

    fscanf(fp, "%d %d",&m.r,&m.c);

    for(i=0; i<m.r && corretto; i++){
        for(j=0; j<m.c; j++){

            if(fscanf(fp,"%d",&m.m[i][j])==1);
            else corretto = 0; //Se il file è ben formattato questo controllo è assolto

        }
    }

    if(corretto) fclose(fp);
    return m;
}

void stampaMatrix(Info m){
 int i,j;

 for(i=0;i<m.r;i++){
    for(j=0;j<m.c; j++){
        printf("%d  ",m.m[i][j]);
    }
    printf("\n");
 }

}

void stampaMatrixFile(Info m){
 int i,j;
 FILE *fp = fopen("out.txt","w");
 for(i=0;i<m.r;i++){
    for(j=0;j<m.c; j++){
        fprintf(fp,"%d  ",m.m[i][j]);
    }
    fprintf(fp,"\n");
 }
fclose(fp);
}

void eseguiMossa (Mossa mossa, Info *matrix){
    int tmp[N],i,j,pos;
    int max = N;

    if(mossa.s){
            max = matrix->r;
            for(i=0; i<max; i++){
                tmp[i]= matrix->m[i][mossa.x];
                printf("%d ",tmp[i]);
            }
    } //MUOVO SU COLONNA (1)
    else {
            max = matrix->c;
            for(i=0; i<max; i++) {
            tmp[i]= matrix->m[mossa.x][i];
            printf("%d ",tmp[i]);
            }
    } //MUOVO SU RIGA (0)
    pos = mossa.p;
    while (pos > max){
        pos=pos-max;
    }
    if (mossa.p==max) pos=0;

     printf("Numero di spostamenti ottimizzati: %d \n",pos);

     if(pos){
    //----------------------
     int delta[2][2] = {
        {0,(pos*mossa.dir)}, /* riga: indice di riga non cambia, spostamento di pos * dir su riga */
        {pos*mossa.dir,0} /* colonna: spostamento di pos * dir su colonna, indice di colonna non cambia  */
    };
    int dx = delta[mossa.s][0]; /* Se 0 mi muovo sulla riga se 1 sulla colonna */
    int dy = delta[mossa.s][1];

    //----------------------

    printf("Conversione mossa: (%d,%d)\n",dx,dy);
    j=0;
    if(dx >0 || dy >0){//Destra o giu

        for(i=0; i<max-pos; i++){
           if(dx==0) matrix->m[mossa.x][i+dy] = tmp[j];
           else matrix->m[i+dx][mossa.x] = tmp[j];
           j = j+1;
        }
        for(i=0; i<pos; i++){
            if(dx==0) matrix->m[mossa.x][i] = tmp[j];
           else matrix->m[i][mossa.x] = tmp[j];
           j = j+1;
        }
    }
    else{
            for(i=max-1; i+pos*mossa.dir>=0; i--){
           if(dx==0) matrix->m[mossa.x][i+dy] = tmp[max-j-1];
           else matrix->m[i+dx][mossa.x] = tmp[max-j-1];
           j = j+1;
        }
    j=0;
        for(i=max+pos*mossa.dir; i<max; i++){
            if(dx==0) matrix->m[mossa.x][i] = tmp[j];
           else matrix->m[i][mossa.x] = tmp[j];
           j = j+1;
        }

    }

    }


return;
}


