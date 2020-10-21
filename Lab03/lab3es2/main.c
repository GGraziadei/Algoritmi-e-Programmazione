#include <stdio.h>
#include <stdlib.h>

#define VISUALIZZA_MEM 0 /* ?visualizza la disposizione dei bit in memoria */
#define NUM_TEST -2130706177

typedef enum{
float_type,double_type,long_double_type,error_type}sel_type;

/*int  32 bit -------- -------- -------- --------
  char  8 bit --------
  Un char *p nella lettura di un int punta al byte di indirizzo minore

  FLOAT: 4bytes (32 bits)
  DOUBLE:8bytes (64 bits)
  LONG DOUBLE:12bytes (96 bits) [2 byte di padding]

  */

void stampaCodifica (void *p, int size, int bigEndian);
int typeExp (int size, int*);

int main()
{
    int Saf,Sad,Sald,bigEndian=NUM_TEST; /*
    Se bigEndian 10000001  00000000  00000000  11111111  Leggendo il primo byte in CA2 questo deve quindi valere -2^7+1
    Se littleEndian 11111111  00000000  00000000  10000001 Leggendo il primo byte in CA2 questo deve quindi valere -1 */

    if( *((char *)&bigEndian)==-1)
        bigEndian = 0;
    float af;
    double ad;
    long double ald;
    Saf = sizeof(af);
    Sad = sizeof(ad);
    Sald = sizeof(ald);
    printf("Tecnica di rappresentazione multicelle: %s",bigEndian?"BIG ENDIAN":"LITTLE ENDIAN");
    printf("\nFLOAT:%dbytes (%d bits)\nDOUBLE:%dbytes (%d bits)\nLONG DOUBLE:%dbytes (%d bits)\n",
           Saf,Saf*8,Sad,Sad*8,Sald,Sald*8);

    printf("Inserire numero da analizzare: ");
    scanf("%f",&af);
    /*Avrei voluto leggere il dato come long double ma non viene riconosciuto %Lf
    Procedo quindi a cast*/
    stampaCodifica((void *)&af,Saf,bigEndian);
    ad=(double)af;
    stampaCodifica((void *)&ad,Sad,bigEndian);
    ald=(long double)af;
    stampaCodifica((void *)&ald,Sald,bigEndian);

    return 0;
}


void stampaCodifica (void *p, int size, int bigEndian){
    unsigned char *numP = p;
    int numV,e,i=0,j=0,exp,countBit=1,paddingByte;
    exp = typeExp(size,&paddingByte);

    i = 1+paddingByte;

    /* i indica il byte in lettura
    j il bit in lettura*/

      while(i<=size){//Byte i-esimo
        /*Scelgo l'ordine in lettura del byte*/
        if(!bigEndian)
         numP = p+size-i;
        else
          numP = p+i-1;
        numV = *numP;
        if(VISUALIZZA_MEM)printf(" %d => %d  ",i,numV);
        i = i+1;
        /*Calcolo i bit notevoli del byte */
        countBit =2; /*(2)base10 = (10)base2 quindi 2 bit*/
        for(e=2; 2*e<=numV; e=e*2)
          countBit++;


        while((8-countBit)!=0){
            printf("0");
            j=j+1;
            countBit++;
             if(j==1 || j==1+exp)
                printf(" ");
             if(paddingByte && j==exp+2)
                printf(" ");/*x86 extended precision*/
        }

        while(e>0){
           if(e<=numV){
             printf("1");
             numV=numV%e;
            }
            else {
              printf("0");
            }

            e = e/2;
            j=j+1; //indica il bit

              if(j==1 || j==1+exp )
                printf(" ");
              if(paddingByte && j==exp+2)
                printf(" ");

        }
      }

return;
}

int typeExp (int size, int *padding){

    int mantissa=0,esponente=0; /*bit per ogni informazione*/
    sel_type type = float_type;
    int ieee_754[3]={4,8,12};/* long double codificati su 10byte*/
    char type_ieee_754[3][30]={"FLOAT","DOUBLE","LONG DOUBLE"};
    while(type<=error_type && size!=ieee_754[type])
        type=type+1;


    switch (type){
     case float_type:esponente=8;
      break;
     case double_type:esponente=11;
      break;
     case long_double_type:esponente=15;
      break;
      case error_type:exit(-1);
    }

    if(type==long_double_type)
        *padding =2;
    else *padding=0;

    mantissa = size*8-esponente-1-*padding*8;
    printf("\nCODIFICA: %s\nBits esponente: %d Bits mantissa: %d Padding bits: %d\n",type_ieee_754[type],esponente,mantissa,*padding*8);


    return esponente;
}
