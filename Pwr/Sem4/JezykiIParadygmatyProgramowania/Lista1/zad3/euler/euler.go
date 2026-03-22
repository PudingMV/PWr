package euler

type Pair struct {
	First int
	Second int
}

func GCD(a, b int) int {
	if b == 0 {
		return a
	}

	return GCD(b, a%b)
}

func MinimalDivisor(n int) int {
	if n%2 == 0 {
		return 2
	}
	for i := 3; i*i <= n; i += 2 {
		if n%i == 0 {
			return i
		}
	}

	return n
}

func Totient(n int) int {
	ans := 1
	for i := 2; i < n; i++ {
		if GCD(i, n) == 1 {
			ans++
		}
	}
	return ans
}

func Diophantic(a, b int) Pair {

	c := GCD(a,b)

	for y := 0; y <= 10000; y++{
		test := c + b*y

		if test % a != 0 {
			continue
		}

		x_candidate := test / a

		if x_candidate > 0 && y > 0 && a*x_candidate - b*y == c {
			return Pair{First: x_candidate, Second: y}			
		}
	}

	return Pair{First: -1, Second: -1}
}