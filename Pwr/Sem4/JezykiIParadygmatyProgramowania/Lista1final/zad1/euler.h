#ifndef EULER_H
#define EULER_H

typedef struct
{
    int first;
    int second;
} Pair;

int NWD(int a, int b);
int minimalDivisor(int n);
int Totient(int n);
Pair diophantic(int a, int b);


#endif