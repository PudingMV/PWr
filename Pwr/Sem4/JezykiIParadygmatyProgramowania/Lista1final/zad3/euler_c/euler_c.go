package main

import "C"
import "zad3/euler"

//export goGCD
func goGCD(a, b C.int) C.int {
    return C.int(euler.GCD(int(a), int(b)))
}

//export goMinimalDivisor
func goMinimalDivisor(n C.int) C.int {
    return C.int(euler.MinimalDivisor(int(n)))
}

//export goTotient
func goTotient(n C.int) C.int {
    return C.int(euler.Totient(int(n)))
}

//export goDiophantic
func goDiophantic(a, b C.int, x *C.int, y *C.int) {
    p := euler.Diophantic(int(a), int(b))
    *x = C.int(p.First)
    *y = C.int(p.Second)
}

func main() {}