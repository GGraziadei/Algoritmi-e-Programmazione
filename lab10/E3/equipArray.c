#include "equipArray.h"

struct equipArray_s{
 int inUso,vettEq[EQUIP_SLOT];
};
char buf[LEN];
typedef enum {fineModifiche,ADD,REMOVE,none}equip_scelte;
static equip_scelte getUpdateCmd (int inUso);
static int addObj(equipArray_t equipArray, int index);
static int removeObj(equipArray_t equipArray,int index);
static char *equip_scelteSUB_MENU[none]={"Fine modifiche",
                                         "Aggiungi",
                                         "Rimuovi"
                                        };


equipArray_t equipArray_init(){
    equipArray_t equipArray = (equipArray_t) malloc(sizeof (struct equipArray_s));
    equipArray->inUso = 0;
    vettEq_void(equipArray->vettEq);
    return equipArray;
}
void vettEq_void(int *vettEq){
    int i;
    for(i=0; i<EQUIP_SLOT; i++)
        vettEq[i] = VUOTO;
}
void equipArray_free(equipArray_t equipArray){
    free(equipArray);
}
int equipArray_inUse(equipArray_t equipArray){
    int i,inUse=0;
    for(i=0; i<EQUIP_SLOT; i++)
        if (equipArray->vettEq[i] != VUOTO)
            inUse = inUse + 1;
    return inUse;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    int i;
    for(i=0; i<EQUIP_SLOT; i++) {
        fprintf(fp, "%2d > ", i);
        if (equipArray->vettEq[i] != VUOTO)
            invArray_printByIndex(fp, invArray, equipArray_getEquipByIndex(equipArray,i));
        else fprintf(fp, "(VUOTO)\n");
    }
}
int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    if(equipArray->vettEq[index] != VUOTO)return equipArray->vettEq[index];
    return -1;
}
int equipArray_getIndexByEquip(equipArray_t equipArray, int index){
    int i;
    for(i=0; i<EQUIP_SLOT && equipArray->vettEq[i]!=index;i++);
    if(i == EQUIP_SLOT)return -1;
    return i;
}

stat_t equipArray_update(equipArray_t equipArray, invArray_t invArray){
    equip_scelte cmd = none;
    int index;
    stat_t delta_stat; stat_void(&delta_stat,0);
    while (cmd != fineModifiche){
        cmd = equipArray_inUse(equipArray);
       cmd = getUpdateCmd(cmd);
       switch (cmd) {
            case ADD:{
                fprintf(stdout,"\nINVENTARIO:\n");
                invArray_print(stdout,invArray);
                fprintf(stdout,"Quale oggetto vuoi inserire in equipaggimaneto ? (indica il nome)\n:");
                fscanf(stdin,"%s",buf);
                index = invArray_searchByName(invArray,buf);
                if(addObj(equipArray,index))
                    delta_stat = stat_sum(&delta_stat,&invArray_getByIndex(invArray,index)->stat);
            } break;
            case REMOVE:{
                fprintf(stdout,"\nEQUIPAGGIAMENTO:\n");
                equipArray_print(stdout,equipArray,invArray);
                fprintf(stdout,"Quale oggetto dell'equipaggiamento vuoi liberare ? (indica il nome)\n:");
                fscanf(stdin,"%s",buf);
                index = invArray_searchByName(invArray,buf);
                if(removeObj(equipArray,index))
                    delta_stat = stat_min(&delta_stat,&invArray_getByIndex(invArray,index)->stat);;
            } break;
           case fineModifiche:cmd = fineModifiche;
               break;
           default:break;
       }
       if(!MULTIPLE_UPDATES)cmd = fineModifiche;
    }
    return delta_stat;
}

static int addObj(equipArray_t equipArray, int index){
    int i;
    if(index == -1){
        fprintf(stdout,"ADD non riuscito, per favore inserisci oggetto presente in inventario\n");
        return 0;
    }
    if (equipArray_getIndexByEquip(equipArray,index) != -1){
        fprintf(stdout,"NON BARARE! Oggetto presente in equipaggiamento\n");
        return 0;
    }
    for(i=0; i<EQUIP_SLOT && equipArray->vettEq[i]!=VUOTO; i++);
    equipArray->vettEq[i] = index;
    equipArray->inUso++;
    return 1;
}
static int removeObj(equipArray_t equipArray,int index){
    int i;
    for(i=0; i<EQUIP_SLOT && index!=equipArray_getEquipByIndex(equipArray,i); i++);
    if(index == -1 || i==EQUIP_SLOT){
        fprintf(stdout,"REMOVE non riuscito, per favore inserisci oggetto presente in equipaggiamento\n");
        return 0;
    }
    equipArray->vettEq[i] = VUOTO;
    equipArray->inUso--;
    return 1;
}
static equip_scelte getUpdateCmd (int inUso){
    /*Operazioni di modifica lecite*/
    int *scelte_disp = calloc(none,sizeof (int ));
    scelte_disp[fineModifiche] = 1;
    if (inUso<EQUIP_SLOT)
        scelte_disp[ADD] = 1;
    if (inUso>0)
        scelte_disp[REMOVE] = 1;
    equip_scelte selezione;
    do {
        fprintf(stdout, "Modifica equipaggiamento:\n");
        for (inUso = 0; inUso < none; inUso++)
            if (scelte_disp[inUso])
                printf("%2d > %s\n",inUso,equip_scelteSUB_MENU[inUso]);
        fscanf(stdin, "%d",&selezione);
    } while (selezione < fineModifiche || selezione>=none || (scelte_disp[selezione] == 0));
    free(scelte_disp);
    return selezione;
}


void equipArray_cpy (equipArray_t dest,equipArray_t src){
    int i;
    dest->inUso = src->inUso;
    for(i=0; i<EQUIP_SLOT; i++)
        dest->vettEq[i] = src->vettEq[i];
}