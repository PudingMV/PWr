#include "wrapper.h"

extern int adaGCD(int a, int b);
extern int adaMinimalDivisor(int n);
extern int adaTotient(int n);
extern Pair adaDiophantic(int a, int b);

extern void goDiophantic(int a, int b, int* x, int* y);
extern int goGCD(int a, int b);
extern int goMinimalDivisor(int n);
extern int goTotient(int n);

Pair goDiophantic_wrapper(int a, int b) {
    Pair p;
    goDiophantic(a, b, &p.first, &p.second);
    return p;
}