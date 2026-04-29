generic
    type T is private;

    with function "*" (A, B : T) return T is <>;
    with function "/" (A, B : T) return T is <>;
    with function FromInt (V : Integer) return T is <>;
    with function ToInt (A : T) return Integer is <>;
    with function Power (A : T; B : Natural) return T is <>;
    with function GetGenerator return T is <>;

    Modulo : Positive;

package User_Generic is

    type User is private;

    function Create return User;

    function GetPublicKey (U : User) return T;
    procedure SetKey (U : in out User; A : T);

    function Encrypt (U : User; M : T) return T;
    function Decrypt (U : User; C : T) return T;

    function GetSecret (U : User) return Natural;
    function GetKey (U : User) return T;

private
    type User is record
        Secret : Natural;
        Key    : T;
        KeySet : Boolean := False;
    end record;

end User_Generic;