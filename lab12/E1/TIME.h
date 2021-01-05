//
// Created by Gianluca Graziadei on 30/12/2020.
//
#ifndef E1_TIME_H
#define E1_TIME_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
typedef struct {
    int gg,mm,yyyy;
}DATA;
typedef struct {
    int hh,mm;
}ORA;
DATA DAY_void();
ORA TIME_void();
ORA TIME_init(FILE *fin);
int DAY_cmp(DATA d1,DATA d2);
void DAY_print(FILE *fout,DATA day);
void TIME_print(FILE *fout,ORA time);
DATA DAY_init(FILE *fin);
int DAY_overlap(DATA d1, DATA test, DATA d2);
#endif //E1_TIME_H
