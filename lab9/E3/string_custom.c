#include "string_custom.h"
#include <ctype.h>
void toMaiuscolo(char *str){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]>='a')
            str[i] = toupper(str[i]);
}
void toMinuscolo (char *str){
    int i,l = strlen(str);
    for(i=0; i<l; i++)
        if(str[i]<'a')
            str[i] = tolower(str[i]);
}