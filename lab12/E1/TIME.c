#include "TIME.h"

ORA TIME_void(){
    ORA time;
    time.hh = time.mm = 0;
    return time;
}
DATA DAY_void(){
    DATA day;
    day.mm = day.gg = day.yyyy = 0;
    return day;
}

ORA TIME_init(FILE *fin){
    assert(fin != NULL);
    ORA time = TIME_void();
    assert(fscanf(fin,"%d:%d",&time.hh,&time.mm) == 2);
    return time;
}

DATA DAY_init(FILE *fin){
    assert(fin != NULL);
    DATA day = DAY_void();
    assert(fscanf(fin,"%d/%d/%d",&day.yyyy,&day.mm,&day.gg) == 3);
    return day;
}

int DAY_cmp(DATA d1,DATA d2){
    if(d1.yyyy != d2.yyyy)
        return d1.yyyy-d2.yyyy;
    else if(d1.mm != d2.mm)
        return d1.mm - d2.mm;
    else return d1.gg - d2.gg;
}

void DAY_print(FILE *fout,DATA day){
    fprintf(fout,"\nDAY\t%04d/%02d/%02d\n",day.yyyy,day.mm,day.gg);
}
void TIME_print(FILE *fout,ORA time){
    fprintf(fout,"%02d:%02d\n",time.hh,time.mm);
}
int DAY_overlap(DATA d1, DATA test, DATA d2){
    return (DAY_cmp(d1,test)<=0 && DAY_cmp(d2,test)>=0);
}