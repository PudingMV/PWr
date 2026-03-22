package maineuler

import (
	"fmt"
	"euler"
)

func main() {
	fmt.Println("GCD 24 and 36 is", euler.GCD(24,36))
	fmt.Println("Minimal divisor of 91 is", euler.MinimalDivisor(91))
	fmt.Println("Totient of 11 is", euler.Totient(11))
	p := euler.Diophantic(15, 10)
	fmt.Println("Diophantic solution:", p.First, p.Second)
}