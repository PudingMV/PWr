generic
    type T is private;

    with function "*" (A, B : T) return T is <>;
    with function FromInt (V : Integer) return T is <>;
    with function ToInt (A : T) return Integer is <>;

    Modulo : Positive;

package DHSetup_Generic is

    function Power (A : T; B : Natural) return T;
    function GetGenerator return T;

private
    Generator : T;

end DHSetup_Generic;