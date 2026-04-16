generic
    Modulo : Positive;
package Ring_Generic is

    type Ring is private;

    -- conversions
    function FromInt (V : Integer) return Ring;
    function ToInt (R : Ring) return Integer;

    -- mathematical operators
    function "+" (A, B : Ring) return Ring;
    function "-" (A, B : Ring) return Ring;
    function "*" (A, B : Ring) return Ring;
    function "/" (A, B : Ring) return Ring;

    -- assignement operators
    procedure AddAssign (A : in out Ring; B : Ring);
    procedure SubAssign (A : in out Ring; B : Ring);
    procedure MulAssign (A : in out Ring; B : Ring);
    procedure DivAssign (A : in out Ring; B : Ring);

    -- comparisons
    function "=" (A, B : Ring) return Boolean;
    -- function "/=" (A, B : Ring) return Boolean;
    function "<" (A, B : Ring) return Boolean;
    function ">" (A, B : Ring) return Boolean;
    function "<=" (A, B : Ring) return Boolean;
    function ">=" (A, B : Ring) return Boolean;

    -- streams
    function ToString (R : Ring) return String;
    procedure Get (R : out Ring);

private
    type Ring is record
        Value : Integer;
    end record;

end Ring_Generic;