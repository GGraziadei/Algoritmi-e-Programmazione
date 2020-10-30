#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define S 100+1

char *cercaRegexp (char *src, char *regexp);
int trovaLRegexp (char *s);
int salta (char *s);
int isMatch (char *w, char* r, int n);

int main(int argc, char *argv[])
{
    char *res,regexp[S],src[]="La felicità non è avere quello che si desidera ma desiderare quello che si ha OscarWilde";
    printf("Stringa acquisita: %s\n",src);
    printf("Inserire espressione regolare: ");
    gets(regexp);
    res = cercaRegexp (src,regexp);
    printf("%s",res==NULL?"\nNessuna occorrenza trovata":res);

    return 0;
}

char *cercaRegexp (char *src, char *regexp){
    char wordTest[S];
    int match = 0,i,n;
    n = trovaLRegexp(regexp);

        while (sscanf(src,"%s%n", wordTest,&i)>0 ){
                if(n==strlen(wordTest)){
                    match = isMatch(wordTest,regexp,n);
                    if(match){
                         *(src+n+1)='\0';
                         return (*src==' ')?src+1:src;
                    }
                    /* Uscita non strutturata se trovo un match.
                    Il programma richiede solamente la prima occorrenza trovata */
                }
                src = src + i; /*Test alla prossima parola*/
        }

return (NULL);
}

int isMatch (char *w, char* r, int n){
    int i,ii=0,trovato=0;
    /*ii: carattere ii-esimo della espressione regolare
    i: verifica della i-esima regola su i-esimo carattere*/
    for (i=0; i<n; i++){

        if(!isalpha(w[i]))
            return 0;
        if (w[i]==r[ii] || r[ii]=='.'){
            ii = ii+1;
            continue;
        }


        if(r[ii]== 92){
            ii = ii+1;
                if(r[ii]== 'a' && (w[i]-'a') < 0)
                    return 0;
                if (r[ii]== 'A' && (w[i]-'a') >= 0)
                    return 0;

        }
        else if(r[ii]== '['){
            ii = ii+1;
                if(r[ii]== '^') {
                    ii = ii+1;
                    while (r[ii] != ']'){
                        if(w[i]==r[ii])
                            return 0;
                        ii = ii+1;
                    }
                }
                else {
                    trovato = 0;
                    while (r[ii] != ']'){
                        if(w[i]==r[ii])
                            trovato = 1;
                        ii = ii+1;
                    }
                    if(!trovato)return 0;
                }
        }
        else return 0;
        ii = ii+1;
    }

    return 1;
}

int trovaLRegexp (char s[]){

    int n=0,i=0,l=strlen(s);

    while (i<l){
        n = n+1;
        if(isalpha(s[i]))
            i= i+1;
        else {
            switch (s[i]){
                case 46 : i=i+1;
                    break;
                case 91 : i = i+salta(&s[i]);
                    break;
                case 92: i = i+2;
                    break;
                default : exit(-1);
                    break;
            }
        }
    }
 return n;
}

int salta (char s[]){
    int i;
    for(i=1; *(s+i)!=']'; i++);
    return i+1;
}
