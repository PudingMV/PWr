package Euler is
  type Pair is record
    First : Integer;
    Second : Integer;
  end record;

  function gcd(a, b : Integer) return Integer;
  pragma Export (C, gcd, "adaGCD");
  function minimalDivisor(n : Integer) return Integer;
  pragma Export (C, minimalDivisor, "adaMinimalDivisor");
  function Totient(n : Integer) return Integer;
  pragma Export (C, Totient, "adaTotient");
  function diophantic(a, b : Integer) return Pair;
  pragma Export (C, diophantic, "adaDiophantic");

end Euler;