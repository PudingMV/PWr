with Ada.Text_IO; use Ada.Text_IO;
with Ring_Generic;

procedure Main is

    package Ring5 is new Ring_Generic(5);
    use Ring5;

    A, B, C, X : Ring;

begin
    Put_Line("Test in Ring modulo 5");
    New_Line;

    Put_Line("Enter a number for Ring (mod 5): ");
    Get(X);

    Put_Line("You entered: " & ToString(X));

    -- Construction/Conversion
    Put_Line("Construction & Conversion");

    A := FromInt(4);
    B := FromInt(2);
    C := FromInt(-1);

    Put_Line("A = FromInt(4)  -> " & ToString(A));
    Put_Line("B = FromInt(2)  -> " & ToString(B));
    Put_Line("C = FromInt(-1) -> " & ToString(C) & " (should be 4)");
    New_Line;

    -- mathematical operators
    Put_Line("[2] Arithmetic operators");

    Put_Line("A + B = " & ToString(A + B));
    Put_Line("A - B = " & ToString(A - B));
    Put_Line("A * B = " & ToString(A * B));
    Put_Line("A / B = " & ToString(A / B));
    New_Line;

    -- assignment operators
    Put_Line("Assignment operators");

    A := FromInt(4);

    AddAssign(A, B);
    Put_Line("A += B -> " & ToString(A));

    SubAssign(A, B);
    Put_Line("A -= B -> " & ToString(A));

    MulAssign(A, B);
    Put_Line("A *= B -> " & ToString(A));

    DivAssign(A, B);
    Put_Line("A /= B -> " & ToString(A));
    New_Line;

    -- comparison operators
    Put_Line("Comparison operators");

    A := FromInt(3);
    B := FromInt(2);

    Put_Line("A = " & ToString(A) & ", B = " & ToString(B));

    Put_Line("A = B  -> " & Boolean'Image(A = B));
    Put_Line("A < B  -> " & Boolean'Image(A < B));
    Put_Line("A > B  -> " & Boolean'Image(A > B));
    Put_Line("A <= B -> " & Boolean'Image(A <= B));
    Put_Line("A >= B -> " & Boolean'Image(A >= B));
    New_Line;

    -- ToString function
    Put_Line("[5] ToString function");

    A := FromInt(7);
    Put_Line("ToString(A = 7 mod 5) -> " & ToString(A));
    New_Line;

    -- exception handling
    Put_Line("Exception test (no modular inverse)");

    declare
        package Ring6 is new Ring_Generic(6);

        X : Ring6.Ring := Ring6.FromInt(2);
        Y : Ring6.Ring := Ring6.FromInt(3);
    begin
        Put_Line("Trying: 2 / 3 in Z6 ...");
        Put_Line("Result: " & Integer'Image(Ring6.ToInt(Ring6."/"(X, Y))));
    exception
        when Constraint_Error =>
            Put_Line("Caught exception: modular inverse does not exist");
    end;

end Main;