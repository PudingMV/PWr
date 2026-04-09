#ifndef WRAPPER_H
#define WRAPPER_H

typedef struct {
    int first;
    int second;
} Pair;

int adaGCD(int a, int b);
int adaMinimalDivisor(int n);
int adaTotient(int n);
Pair adaDiophantic(int a, int b);

int goGCD(int a, int b);
int goMinimalDivisor(int n);
int goTotient(int n);
void goDiophantic(int a, int b, int* x, int* y);

Pair goDiophantic_wrapper(int a, int b);

#endif