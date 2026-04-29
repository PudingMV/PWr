with Ada.Numerics.Discrete_Random;

package body DHSetup_Generic is

    function Power (A : T; B : Natural) return T is
        Result : T := FromInt(1);
        Base   : T := A;
        Exp    : Natural := B;
    begin
        while Exp > 0 loop
            if Exp mod 2 = 1 then
                Result := Result * Base;
            end if;
            Base := Base * Base;
            Exp := Exp / 2;
        end loop;
        return Result;
    end Power;

    type Int_Array is array (Natural range <>) of Integer;

    function PrimeFactors (N : Integer) return Int_Array is
        Temp : Integer := N;
        Factors : Int_Array(1 .. 64); -- wystarczy
        Count : Natural := 0;
        I : Integer := 2;
    begin
        while I * I <= Temp loop
            if Temp mod I = 0 then
                Count := Count + 1;
                Factors(Count) := I;
                while Temp mod I = 0 loop
                    Temp := Temp / I;
                end loop;
            end if;
            I := I + 1;
        end loop;

        if Temp > 1 then
            Count := Count + 1;
            Factors(Count) := Temp;
        end if;

        return Factors(1 .. Count);
    end PrimeFactors;

    function IsGenerator (A : T) return Boolean is
        PMinus1 : constant Integer := Modulo - 1;
        Factors : Int_Array := PrimeFactors(PMinus1);
    begin
        for Q of Factors loop
            if ToInt(Power(A, Natural(PMinus1 / Q))) = 1 then
                return False;
            end if;
        end loop;

        return True;
    end IsGenerator;

    subtype Range_Type is Integer range 2 .. Modulo - 2;
    package Rand is new Ada.Numerics.Discrete_Random(Range_Type);
    Gen : Rand.Generator;

    function RandomElement return T is
    begin
        return FromInt(Rand.Random(Gen));
    end;

    function FindGenerator return T is
    begin
        for I in 1 .. 10000 loop
            declare
                Candidate : T := RandomElement;
            begin
                if IsGenerator(Candidate) then
                    return Candidate;
                end if;
            end;
        end loop;

        raise Constraint_Error with "No generator found";
    end;

    function GetGenerator return T is
    begin
        return Generator;
    end;

begin
    Rand.Reset(Gen);
    Generator := FindGenerator;

end DHSetup_Generic;