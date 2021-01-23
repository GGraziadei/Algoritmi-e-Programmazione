#include <stdio.h>
#include "CorpoLibero.h"
#define ELEMENTI "elementi.txt"
#define DBG 1


typedef struct {
    ELMENTS elmentsArray;
    int dd,dp;
}tab_t;

tab_t init_DB();
void free_DB(tab_t *tab);

int main() {
    setbuf(stdout,0);
    tab_t DB;
    do {
        DB = init_DB();
        printf("\n\n INSERIRE DD e DP (-1 per uscire):\n> ");
        scanf("%d %d",&DB.dd,&DB.dp);
        if(DB.dd >= 0 && DB.dp>=0){
            CORPOLIBERO_greedy(DB.elmentsArray,DB.dd,DB.dp);
            free_DB(&DB);
        }
    }while (DB.dd >= 0);
    return 0;
}

tab_t init_DB(){
    int N;
    tab_t tab;
    tab.dd = 0;
    tab.dp = 0;
    FILE *fp = fopen(ELEMENTI,"r");
    fscanf(fp,"%d",&N);
    tab.elmentsArray = ELMENTS_init(N);
    ELMENTS_read(fp,tab.elmentsArray);
    #if DBG
    ELMENTS_print(stdout,tab.elmentsArray);
    #endif
    fclose(fp);
    return tab;
}

void free_DB(tab_t *tab){
    ELMENTS_free(tab->elmentsArray);
}
