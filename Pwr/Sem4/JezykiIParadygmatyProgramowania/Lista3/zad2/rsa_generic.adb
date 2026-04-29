package body RSA_Generic is

    use R;

    function GCD (A, B : Natural) return Natural is
        X : Natural := A;
        Y : Natural := B;
        T : Natural;
    begin
        while Y /= 0 loop
            T := Y;
            Y := X mod Y;
            X := T;
        end loop;
        return X;
    end GCD;

    function Find_Coprime (Phi : Positive) return Positive is
    begin
        for I in 3 .. Phi - 1 loop
            if GCD(I, Phi) = 1 then
                return I;
            end if;
        end loop;
        raise Constraint_Error;
    end;

    function Mod_Inv (A, M : Integer) return Integer is
        T, NewT : Integer := 0;
        R, NewR : Integer := M;
        Q, Tmp  : Integer;
    begin
        NewT := 1;
        NewR := A;

        while NewR /= 0 loop
            Q := R / NewR;

            Tmp := T - Q * NewT;
            T := NewT;
            NewT := Tmp;

            Tmp := R - Q * NewR;
            R := NewR;
            NewR := Tmp;
        end loop;

        if R /= 1 then
            raise Constraint_Error;
        end if;

        if T < 0 then
            T := T + M;
        end if;

        return T;
    end;

    function Mod_Pow (Base : R.Ring; Exp : Natural) return R.Ring is
        Result : R.Ring := R.FromInt(1);
        B      : R.Ring := Base;
        E      : Natural := Exp;
    begin
        while E > 0 loop
            if (E mod 2) = 1 then
                Result := Result * B;
            end if;

            B := B * B;
            E := E / 2;
        end loop;

        return Result;
    end;

    procedure Init (Obj : in out RSA; P, Q : Positive) is
    begin
        Obj.N   := P * Q;
        Obj.Phi := (P - 1) * (Q - 1);

        Obj.E := Find_Coprime(Obj.Phi);
        Obj.D := Mod_Inv(Obj.E, Obj.Phi);
    end;

    function GetModulo (Obj : RSA) return Positive is
    begin
        return Obj.N;
    end;

    function GetPublicKey (Obj : RSA) return Integer is
    begin
        return Obj.E;
    end;

    function Encrypt (Obj : RSA; M : R.Ring) return R.Ring is
    begin
        return Mod_Pow(M, Obj.E);
    end;

    function Decrypt (Obj : RSA; S : R.Ring) return R.Ring is
    begin
        return Mod_Pow(S, Obj.D);
    end;

end RSA_Generic;