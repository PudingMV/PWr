package main

import "C"
import "zad3/euler"

func GoGCD(a, b C.int) C.int {
    return C.int(euler.GCD(int(a), int(b)))
}

func GoMinimalDivisor(n C.int) C.int {
    return C.int(euler.MinimalDivisor(int(n)))
}

func GoTotient(n C.int) C.int {
    return C.int(euler.Totient(int(n)))
}

type CPair struct {
    First  C.int
    Second C.int
}

func GoDiophantic(a, b C.int) CPair {
    p := euler.Diophantic(int(a), int(b))
    return CPair{First: C.int(p.First), Second: C.int(p.Second)}
}

func main() {}