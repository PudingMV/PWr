with Ada.Numerics.Discrete_Random;

package body User_Generic is

    subtype Range_Type is Integer range 2 .. Modulo - 2;
    package Rand is new Ada.Numerics.Discrete_Random(Range_Type);
    Gen : Rand.Generator;

    function RandomSecret return Natural is
    begin
        return Natural(Rand.Random(Gen));
    end;

    function Create return User is
        U : User;
    begin
        Rand.Reset(Gen);
        U.Secret := RandomSecret;
        return U;
    end;

    function GetPublicKey (U : User) return T is
    begin
        return Power(GetGenerator, U.Secret);
    end;

    procedure SetKey (U : in out User; A : T) is
    begin
        U.Key := Power(A, U.Secret);
        U.KeySet := True;
    end;

    function Encrypt (U : User; M : T) return T is
    begin
        if not U.KeySet then
            raise Constraint_Error with "Key not set";
        end if;

        return M * U.Key;
    end;

    function Decrypt (U : User; C : T) return T is
    begin
        if not U.KeySet then
            raise Constraint_Error with "Key not set";
        end if;

        return C / U.Key;
    end;

    function GetSecret (U : User) return Natural is
   begin
      return U.Secret;
   end;

   function GetKey (U : User) return T is
   begin
      return U.Key;
   end;

end User_Generic;