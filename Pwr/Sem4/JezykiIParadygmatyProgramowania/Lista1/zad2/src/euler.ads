package Euler is
  type Pair is record
    First : Integer;
    Second : Integer;
  end record;

  function gcd(a, b : Integer) return Integer;
  function minimalDivisor(n : Integer) return Integer;
  function Totient(n : Integer) return Integer;
  function diophantic(a, b : Integer) return Pair;

end Euler;