package cwrapper

/*export LD_LIBRARY_PATH=./zad1:./zad2:./zad3/euler_c

go build -o test_all main.go c_wrapper.go ada_wrapper.go go_wrapper.go
./test_all*/

import "C"

// C library functions
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
    var x, y C.int
    C.diophantic(C.int(a), C.int(b), &x, &y)
    return int(x), int(y)
}