package gowrapper

/*
#cgo LDFLAGS: -L../zad3/euler_c -lgo_euler
#include <stdint.h>

int32_t goGCD(int32_t a, int32_t b);
int32_t goMinimalDivisor(int32_t n);
int32_t goTotient(int32_t n);
void goDiophantic(int32_t a, int32_t b, int32_t* x, int32_t* y);
*/
import "C"

func GCD_Go(a, b int) int {
    return int(C.goGCD(C.int(a), C.int(b)))
}

func MinimalDivisor_Go(n int) int {
    return int(C.goMinimalDivisor(C.int(n)))
}

func Totient_Go(n int) int {
    return int(C.goTotient(C.int(n)))
}

func Diophantic_Go(a, b int) (int, int) {
    var x, y C.int
    C.goDiophantic(C.int(a), C.int(b), &x, &y)
    return int(x), int(y)
}