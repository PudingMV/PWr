package main

import (
    "fmt"
    "zad6/cwrapper"
    "zad6/adawrapper"
)

func main() {
    a, b := 60, 42
    n := 91

    fmt.Printf("GCD(C)            : %d\n", cwrapper.GCD_C(a, b))
    fmt.Printf("MinimalDivisor(C) : %d\n", cwrapper.MinimalDivisor_C(n))
    fmt.Printf("Totient(C)        : %d\n", cwrapper.Totient_C(n))
    x, y := cwrapper.Diophantic_C(15, 21)
    fmt.Printf("Diophantic(C)     : x=%d, y=%d\n", x, y)

    fmt.Printf("GCD(Ada)          : %d\n", adawrapper.GCD_Ada(a, b))
    fmt.Printf("MinimalDivisor(Ada): %d\n", adawrapper.MinimalDivisor_Ada(n))
    fmt.Printf("Totient(Ada)       : %d\n", adawrapper.Totient_Ada(n))
    x, y = adawrapper.Diophantic_Ada(15, 21)
    fmt.Printf("Diophantic(Ada)    : x=%d, y=%d\n", x, y)
}