with Ring_Generic;
generic
    with package R is new Ring_Generic(<>);
package RSA_Generic is

    type RSA is private;

    procedure Init (Obj : in out RSA; P, Q : Positive);

    function GetModulo (Obj : RSA) return Positive;
    function GetPublicKey (Obj : RSA) return Integer;

    function Encrypt (Obj : RSA; M : R.Ring) return R.Ring;
    function Decrypt (Obj : RSA; S : R.Ring) return R.Ring;

private
    type RSA is record
        N   : Positive;
        Phi : Positive;
        E   : Positive;
        D   : Positive;
    end record;

end RSA_Generic;