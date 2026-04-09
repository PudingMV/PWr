with Interfaces.C; use Interfaces.C;

package Wrapper is

   -- Record to hold Diophantic results
   type Pair is record
      First  : Interfaces.C.int;
      Second : Interfaces.C.int;
   end record;

   function cGCD(A, B : Interfaces.C.int) return Interfaces.C.int;
   pragma Import(C, cGCD, "NWD");

   function cMinimalDivisor(N : Interfaces.C.int) return Interfaces.C.int;
   pragma Import(C, cMinimalDivisor, "minimalDivisor");

   function cTotient(N : Interfaces.C.int) return Interfaces.C.int;
   pragma Import(C, cTotient, "Totient");

   procedure cDiophantic(A, B : Interfaces.C.int; X, Y : out Interfaces.C.int);
   pragma Import(C, cDiophantic, "diophantic");

   function goGCD(A, B : Interfaces.C.int) return Interfaces.C.int;
   pragma Import(C, goGCD, "goGCD");

   function goMinimalDivisor(N : Interfaces.C.int) return Interfaces.C.int;
   pragma Import(C, goMinimalDivisor, "goMinimalDivisor");

   function goTotient(N : Interfaces.C.int) return Interfaces.C.int;
   pragma Import(C, goTotient, "goTotient");

   procedure goDiophantic(A, B : Interfaces.C.int; X, Y : out Interfaces.C.int);
   pragma Import(C, goDiophantic, "goDiophantic");

   function goDiophantic_wrapper(A, B : Interfaces.C.int) return Pair;

end Wrapper;