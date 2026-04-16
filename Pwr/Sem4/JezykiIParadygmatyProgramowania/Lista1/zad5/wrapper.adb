with Interfaces.C; use Interfaces.C;

package body Wrapper is

   function goDiophantic_wrapper(A, B : Interfaces.C.int) return Pair is
      X, Y : Interfaces.C.int;
   begin
      goDiophantic(A, B, X, Y);  -- call imported Go procedure
      return (First => X, Second => Y);
   end goDiophantic_wrapper;

end Wrapper;