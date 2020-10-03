#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define S 100+1
#define N 20
#define M 30


typedef struct Team{
int id;
int punti;
}Team;

typedef struct Campionato{
int teams;
int giornate;
int calendario[N][M];
int modPari;
}Campionato;

Campionato creaCampionato(void);
void stampaIndice(Campionato c, Team t[]);
void creaTeams(Team t[], int n);
int trovaMax (Team t[], int l);

int main()
{
    Campionato c = creaCampionato();
    Team teams[c.teams];
    creaTeams(teams,c.teams);
    stampaIndice(c,teams);

    return 0;
}


Campionato creaCampionato(void){
    int i,j;
    Campionato c;
    char nomeCampionato[S];
    printf("Inserisci il nome del campionato (NOME FILE):");
    scanf("%s",nomeCampionato);


    if(nomeCampionato != NULL) {
            FILE *in = fopen(nomeCampionato,"r");

            if(in == NULL )exit("Errore file non trovato RIAVVIA!");


            fscanf(in,"%d %d",&c.teams,&c.giornate);
            printf("Numero squadre %d numero giornate %d\n",c.teams,c.giornate);
                //Ulteriori controlli sul file sono assolti dall'ipotesi che sia ben formattato
            for(i=0;i<c.teams;i++){
                for(j=0;j<c.giornate; j++){

                    fscanf(in,"%d",&c.calendario[i][j]);
                   //printf("%d  ",c.calendario[i][j]);

                }
              //printf("\n");
            }
            c.modPari = 1; //default :-)
            printf("Creazione campionato riuscita. Vuoi abilitare la MOD PARI (1=SI,0=NO)? \n(gestisce eventuali situazioni di pareggio e stampa tutti i teams PARI): ");
            scanf("%d",&c.modPari);

            fclose(in);
    }
    else exit("Nome file non valido RIAVVIA !");

return c;
}

void creaTeams(Team t[], int n){
    int i;
    for(i=0;i<n;i++){
        t[i].id = i;
        t[i].punti = 0;
    }
return;
}

void stampaIndice(Campionato c, Team t[]){
    int i,j,jj,max,finito;
    printf("Dominio indici [%d , %d]\n",0,c.teams-1);
    for(i=0;i<c.giornate;i++){ //giornata i-esima
     finito =0;
        for(j=0;j<c.teams; j++){ //squadra j-esima

            t[j].punti = t[j].punti + c.calendario[j][i];

        }
            printf("ID CAPOISTA/I (giornata %d): ",i+1);
            max = trovaMax(t,c.teams);
            for(jj=0; jj<c.teams && !finito; jj++){
                if(t[jj].punti == max){
                        printf("%d ",t[jj].id);
                        if(!c.modPari) finito = 1; //MOD PARI
                }
            }printf("\n");
    }
return;
}

int trovaMax (Team t[], int l){
    int i,max=t[0].punti;

    for(i=1;i<l;i++)
        if(max<t[i].punti) max = t[i].punti;

return max;
}
