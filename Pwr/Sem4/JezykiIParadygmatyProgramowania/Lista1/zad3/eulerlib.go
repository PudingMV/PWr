package main

import "C"

import "zad3/euler"

func GCD(a, b C.int) C.int {
	return C.int(euler.GCD(int(a), int(b)))
}

func MinimalDivisor(n C.int) C.int {
	return C.int(euler.MinimalDivisor(int(n)))
}

func Totient(n C.int) C.int {
	return C.int(euler.Totient(int(n)))
}

func Diophantic(a, b C.int) (C.int, C.int) {
	p := euler.Diophantic(int(a), int(b))
	return C.int(p.First), C.int(p.Second)
}

func main() {}