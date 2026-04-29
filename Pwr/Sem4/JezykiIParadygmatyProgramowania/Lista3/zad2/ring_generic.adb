with Ada.Integer_Text_IO;
package body Ring_Generic is

    function Normalize (V : Integer) return Integer is
    begin
        return (V mod Modulo + Modulo) mod Modulo;
    end;

    -- conversions
    function FromInt (V : Integer) return Ring is
    begin
        return (Value => Normalize(V));
    end;
    function ToInt (R : Ring) return Integer is
    begin
        return R.Value;
    end;

    -- inverse euclides
    function ModInverse (A : Integer) return Integer is
        T, NewT : Integer := 0;
        R, NewR : Integer := Modulo;
        Q, Temp : Integer;
    begin
        NewT := 1;
        NewR := A;

        while NewR /= 0 loop
            Q := R / NewR;

            Temp := T - Q * NewT;
            T := NewT;
            NewT := Temp;

            Temp := R - Q * NewR;
            R := NewR;
            NewR := Temp;
        end loop;

        if R /= 1 then
            raise Constraint_Error with "No inverse exists";
        end if;

        if T < 0 then
            T := T + Modulo;
        end if;

        return T;
    end;

    -- mathematical operators
    function "+" (A, B : Ring) return Ring is
    begin
        return FromInt(A.Value + B.Value);
    end;

    function "-" (A, B : Ring) return Ring is
    begin
        return FromInt(A.Value - B.Value);
    end;

    function "*" (A, B : Ring) return Ring is
        Tmp : Long_Long_Integer;
    begin
        Tmp := Long_Long_Integer(A.Value) * Long_Long_Integer(B.Value);
        return FromInt(Integer(Tmp mod Long_Long_Integer(Modulo)));
    end;

    function "/" (A, B : Ring) return Ring is
    begin
        return FromInt(A.Value * ModInverse(B.Value));
    end;

    -- assignement operators
    procedure AddAssign (A : in out Ring; B : Ring) is
    begin
        A := A + B;
    end;

    procedure SubAssign (A : in out Ring; B : Ring) is
    begin
        A := A - B;
    end;

    procedure MulAssign (A : in out Ring; B : Ring) is
    begin
        A := A * B;
    end;

    procedure DivAssign (A : in out Ring; B : Ring) is
    begin
        A := A / B;
    end;

    -- Porównania
    function "=" (A, B : Ring) return Boolean is
    begin
        return A.Value = B.Value;
    end;

    -- function "/=" (A, B : Ring) return Boolean is
    -- begin
        -- return A.Value /= B.Value;
    -- end;

    function "<" (A, B : Ring) return Boolean is
    begin
        return A.Value < B.Value;
    end;

    function ">" (A, B : Ring) return Boolean is
    begin
        return A.Value > B.Value;
    end;

    function "<=" (A, B : Ring) return Boolean is
    begin
        return A.Value <= B.Value;
    end;

    function ">=" (A, B : Ring) return Boolean is
    begin
        return A.Value >= B.Value;
    end;

    -- streams
    function ToString (R : Ring) return String is
    begin
        return Integer'Image(ToInt(R));
    end;

    procedure Get (R : out Ring) is
    V : Integer;
    begin
        Ada.Integer_Text_IO.Get(V);
        R := FromInt(V);
    end Get;

end Ring_Generic;
