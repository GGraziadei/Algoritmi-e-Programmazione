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
    char *res,regexp[S],src[]="Ciao Mondo Hello World";
    printf("Stringa acquisita: %s\n",src);
    printf("Inserire espressione regolare: ");
    gets(regexp);
    res = cercaRegexp (src,regexp);
    printf("%s",res==NULL?"\nNessuna occorrenza trovata":res);

    return 0;
}

char *cercaRegexp (char *src, char *regexp){
    char wordTest[S];
    int match = 0,i,j,n;
    n = trovaLRegexp(regexp);

        while (sscanf(src,"%s %n", wordTest,&i)>0 ){
                for(j=0; j<strlen(wordTest) && n-strlen(&wordTest[j])>=0; j++){
                    //printf("Provo con: %s \n\n",&(wordTest[j]));
                    match = isMatch(&(wordTest[j]),regexp,n);
                    if(match){
                        char *res = &(src[j]);
                        printf("Occorrenza trovata nella parola: %s\nPunto di MATCH: ",wordTest);
                         *(res+n)='\0';
                         return res;
                    }
                    /* Uscita non strutturata se trovo un match.
                    Il programma richiede solamente la prima occorrenza trovata */
                }
                src = src + i; /*Test alla prossima parola*/
        }

return (NULL);
}

int isMatch (char *w, char *r, int n){
    int i,ii=0,trovato=0;
    /*ii: carattere ii-esimo della espressione regolare
    i: verifica della i-esima regola su i-esimo carattere*/
    for (i=0; i<n; i++){
       /*Condizione indicata nel testo dell'esercizio*/
        if(!isalpha(w[i]))
            return 0;
        if (w[i]==r[ii] || r[ii]=='.'){
            ii++;
            continue;
        }
        if(r[ii]== '\\'){
                if(r[++ii]== 'a' && (w[i]-'a') < 0)
                    return 0;
                if (r[ii]== 'A' && (w[i]-'a') >= 0)
                    return 0;
                ii++;
        }
        else if(r[ii] == '['){
            ii++;
            if(r[ii] == '^'){
                trovato = -1;
                 ii++;
             }
            else
                trovato = 0;

            while (r[ii] != ']'){
                    if(w[i]==r[ii] && trovato == -1)
                        return 0;
                    if(w[i]==r[ii] && trovato == 0)
                        trovato = 1;
                    ii++;
            }
            ii++;
            if (trovato == 0)
                return 0;
        }
        else return 0;
    }

    return 1;
}

int trovaLRegexp (char *s){
    int n=0,i=0,l=strlen(s);
    /*Nel conteggiare i caratteri verifico anche la formattazione della regexp*/
    while (i<l){
        n = n+1;
        if(isalpha(s[i]))
            i= i+1;
        else {
            switch (s[i]){
                case '.' : i=i+1;
                    break;
                case '[' : i = i+salta(&s[i]);
                    break;
                case '\\': i = i+2;
                    break;
                default : exit(-1);
            }
        }
    }
 return n;
}

int salta (char *s){
    int i;
    for(i=1; *(s+i)!=']'; i++);
    return i+1;
}
