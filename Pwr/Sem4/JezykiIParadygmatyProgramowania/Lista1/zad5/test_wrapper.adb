with Ada.Text_IO; use Ada.Text_IO;
with Interfaces.C; use Interfaces.C;
with Wrapper; use Wrapper;

procedure Test_Wrapper is
   A, B : Interfaces.C.int := 60;
   N    : Interfaces.C.int := 91;
   P    : Wrapper.Pair;
begin
   Put_Line("GCD(C)            : " & Integer'Image(Integer(cGCD(A, B))));
   Put_Line("MinimalDivisor(C) : " & Integer'Image(Integer(cMinimalDivisor(N))));
   Put_Line("Totient(C)        : " & Integer'Image(Integer(cTotient(N))));
   cDiophantic(15, 21, P.First, P.Second);
   Put_Line("Diophantic(C)     : x=" & Integer'Image(Integer(P.First)) &
            ", y=" & Integer'Image(Integer(P.Second)));

   Put_Line("GCD(Go)           : " & Integer'Image(Integer(goGCD(A, B))));
   Put_Line("MinimalDivisor(Go): " & Integer'Image(Integer(goMinimalDivisor(N))));
   Put_Line("Totient(Go)       : " & Integer'Image(Integer(goTotient(N))));
   P := goDiophantic_wrapper(15, 21);
   Put_Line("Diophantic(Go)    : x=" & Integer'Image(Integer(P.First)) &
            ", y=" & Integer'Image(Integer(P.Second)));
end Test_Wrapper;