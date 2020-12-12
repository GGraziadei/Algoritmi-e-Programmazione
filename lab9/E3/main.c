#include <stdio.h>
#include "string_custom.h"
#include <stdlib.h>
#include <assert.h>
#define S 50+1
#include "inv.h"
#include "pg.h"

typedef enum {
    r_add_pg,r_info_pg,r_delete_pg, r_update_equip,r_remove_equip,r_stat_pg,r_info_obj,r_fine,r_errore
}e_comandi;

e_comandi leggiComando();
void selezionaDati(e_comandi c,link_tabPg,link_tabInv);

int main() {
    setbuf(stdout, 0);
    e_comandi CODcomando = r_add_pg;
    link_tabPg tabPg = init_tabPg();
    link_tabInv tabInv = init_tabInv();
    w_add_PG(tabPg,pg_file);
    w_add_INV(tabInv,inv_file);
    while(CODcomando != r_fine){
        CODcomando = leggiComando();
        if(CODcomando != r_fine)
            selezionaDati (CODcomando,tabPg,tabInv);
    }
    tabPg = libera_tabPG (tabPg);
    tabInv = libera_tabINV (tabInv);

    return 0;
}
void selezionaDati(e_comandi c,link_tabPg tabPg,link_tabInv tabInv){
/*Comandi disponibili   r_add_pg,r_delete_pg, r_update_equip,r_stat_pg,r_fine,r_errore*/
    assert(c<r_fine && c>=r_add_pg);
    switch (c) {
        case r_add_pg: w_add_PG(tabPg,pg_keyboard);
            break;
        case r_delete_pg: w_delete_PG(tabPg);
            break;
        case r_update_equip: w_update_Equip(tabInv,tabPg);
            break;
        case r_stat_pg: w_stat(tabPg);
            break;
        case r_remove_equip: w_remove_Equip(tabInv,tabPg);
            break;
        case r_info_pg: w_info_PG(tabPg);
            break;
        case r_info_obj: w_info_obj(tabInv);
            break;
        default:exit(-1);
    }
}

e_comandi leggiComando(){
    int i;
    char cmd[S];
    e_comandi c;
    char *c_tabella[r_errore]={ "add_pg","info_pg","delete_pg", "update_equip","remove_equip","stat","info_obj","fine" };
    char *c_tabella_Comandi[r_errore]={
            "\t1. Aggiungi personaggio: add_pg",
            "\t2. Info personaggio se presente in lista: info_pg",
            "\t3. Cancella personaggio: delete_pg <code>",
            "\t4. Aggiorna equipaggiamento di un personaggio: update_equip <code>",
            "\t5. Elimina oggetto equipaggiamento di un personaggio: remove_equip <code>",
            "\t6. Statistische personaggio: stat <code>",
            "\t6. Info oggetto: info_obj <name>",
            "\tTermina esecuzione: fine"
    };
    do{
        printf("\t\tMENU DI SELEZIONE\n");
        for(i=0; i<r_errore; i++)
            printf("%s \n",c_tabella_Comandi[i] );
        printf("\tInserire comando come da specifiche sopra indicate: ");

        if(scanf("%s",cmd)==1){
            toMinuscolo(cmd);
            c = r_add_pg;
            while(c<r_errore && strcmp(c_tabella[c],cmd)!=0 )
                c++;
        }
        if(c==r_errore)
            printf("Comando non valido\n\n");
    }while(c == r_errore);
    return c;
}















































