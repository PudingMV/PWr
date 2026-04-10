import c_wrapper
import ada_wrapper
import go_wrapper

a, b = 60, 42
n = 91

print("GCD_C:", c_wrapper.gcd(a, b))
print("MinDiv_C:", c_wrapper.minimal_divisor(n))
print("Totient_C:", c_wrapper.totient(n))
print("Diophantic_C:", c_wrapper.diophantic(15, 21))

print("\n")
print("GCD_ADA:", ada_wrapper.gcd(a, b))
print("MinDiv_ADA:", ada_wrapper.minimal_divisor(n))
print("Totient_ADA:", ada_wrapper.totient(n))
print("Diophantic_ADA:", ada_wrapper.diophantic(15, 21))

print("\n")
print("GCD_GO:", go_wrapper.gcd(a, b))
print("MinDiv_GO:", go_wrapper.minimal_divisor(n))
print("Totient_GO:", go_wrapper.totient(n))
print("Diophantic_GO:", go_wrapper.diophantic(15, 21))