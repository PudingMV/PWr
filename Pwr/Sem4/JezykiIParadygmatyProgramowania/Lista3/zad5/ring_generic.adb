with Ada.Integer_Text_IO;

package body Ring_Generic is

    subtype LL is Long_Long_Integer;

    function Normalize (V : LL) return LL is
    begin
        return ((V mod LL(Modulo)) + LL(Modulo)) mod LL(Modulo);
    end;

    function FromInt (V : Integer) return Ring is
    begin
        return (Value => Normalize(LL(V)));
    end;

    function ToInt (R : Ring) return Integer is
    begin
        return Integer(R.Value);
    end;

    function ModInverse (A : LL) return LL is
        T, NewT : LL := 0;
        R, NewR : LL := LL(Modulo);
        Q, Temp : LL;
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
            T := T + LL(Modulo);
        end if;

        return T;
    end;

    function "+" (A, B : Ring) return Ring is
    begin
        return (Value => Normalize(A.Value + B.Value));
    end;

    function "-" (A, B : Ring) return Ring is
    begin
        return (Value => Normalize(A.Value - B.Value));
    end;

    function "*" (A, B : Ring) return Ring is
    begin
        return (Value =>
            Normalize((A.Value mod LL(Modulo)) *
                      (B.Value mod LL(Modulo))));
    end;

    function "/" (A, B : Ring) return Ring is
        Inv : LL := ModInverse(B.Value);
    begin
        return (Value => Normalize(A.Value * Inv));
    end;

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

    function "=" (A, B : Ring) return Boolean is
    begin
        return A.Value = B.Value;
    end;

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

    function ToString (R : Ring) return String is
    begin
        return Integer'Image(Integer(R.Value));
    end;

    procedure Get (R : out Ring) is
        V : Integer;
    begin
        Ada.Integer_Text_IO.Get(V);
        R := FromInt(V);
    end;

end Ring_Generic;