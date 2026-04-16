package cwrapper

/*
#cgo CFLAGS: -I${SRCDIR}/../../zad1
#cgo LDFLAGS: -L${SRCDIR}/../../zad1 -leuler

#include "../zad1/euler.h"
*/
import "C"

type Pair struct {
    First  C.int
    Second C.int
}

func GCD_C(a, b int) int {
    return int(C.NWD(C.int(a), C.int(b)))
}

func MinimalDivisor_C(n int) int {
    return int(C.minimalDivisor(C.int(n)))
}

func Totient_C(n int) int {
    return int(C.Totient(C.int(n)))
}

func Diophantic_C(a, b int) (int, int) {
    p := C.diophantic(C.int(a), C.int(b))
    return int(p.first), int(p.second)
}