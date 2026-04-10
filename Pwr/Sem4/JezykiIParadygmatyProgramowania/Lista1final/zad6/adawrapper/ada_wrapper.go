package adawrapper

/*
#cgo LDFLAGS: -L${SRCDIR}/../../zad2 -lada_euler
#include <stdint.h>

// funkcje C-callable z Ada
int32_t adaGCD(int32_t a, int32_t b);
int32_t adaMinimalDivisor(int32_t n);
int32_t adaTotient(int32_t n);
void adaDiophantic(int32_t a, int32_t b, int32_t* x, int32_t* y);
*/
import "C"

func GCD_Ada(a, b int) int {
    return int(C.adaGCD(C.int(a), C.int(b)))
}

func MinimalDivisor_Ada(n int) int {
    return int(C.adaMinimalDivisor(C.int(n)))
}

func Totient_Ada(n int) int {
    return int(C.adaTotient(C.int(n)))
}

func Diophantic_Ada(a, b int) (int, int) {
    var x, y C.int
    C.adaDiophantic(C.int(a), C.int(b), &x, &y)
    return int(x), int(y)
}