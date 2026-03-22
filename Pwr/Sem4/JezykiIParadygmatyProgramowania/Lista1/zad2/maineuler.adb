with Ada.Text_IO; use Ada.Text_IO;
with Euler;

procedure maineuler is
  P : Euler.Pair;
begin
  Put_Line("GCD of 24 and 36 is" & Integer'Image(Euler.gcd(24, 36)));
  Put_Line("Minimal divisor of 91 is" & Integer'Image(Euler.minimalDivisor(91)));
  Put_Line("Totient of 11 is" & Integer'Image(Euler.Totient(11)));
  P := Euler.diophantic(15, 21);
  Put_Line("Diophantic solution x =" & Integer'Image(P.First) & ", y =" & Integer'Image(P.second));
end maineuler;