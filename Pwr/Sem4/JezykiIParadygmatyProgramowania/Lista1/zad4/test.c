#include <stdio.h>
#include "wrapper.h"

int main() {
    int a = 60, b = 48, n = 91;
    Pair p;

    printf("GCD Ada = %d\n", adaGCD(a,b));
    printf("GCD Go  = %d\n", goGCD(a,b));

    printf("Minimal divisor Ada = %d\n", adaMinimalDivisor(n));
    printf("Minimal divisor Go  = %d\n", goMinimalDivisor(n));

    printf("Euler Totient Ada = %d\n", adaTotient(n));
    printf("Euler Totient Go  = %d\n", goTotient(n));

    p = adaDiophantic(15, 21);
    printf("Ada Diophantic: x=%d, y=%d\n", p.first, p.second);

    p = goDiophantic_wrapper(15, 21);
    printf("Go Diophantic: x=%d, y=%d\n", p.first, p.second);

    return 0;
}