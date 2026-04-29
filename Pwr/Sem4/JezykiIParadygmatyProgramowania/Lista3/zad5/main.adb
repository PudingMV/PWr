with Ada.Text_IO; use Ada.Text_IO;

with Ring_Generic;
with DHSetup_Generic;
with User_Generic;

procedure Main is

    package R is new Ring_Generic(1234567891);

    package DH is new DHSetup_Generic
      (T => R.Ring,
       "*" => R."*",
       FromInt => R.FromInt,
       ToInt => R.ToInt,
       Modulo => 1234567891);

    package U is new User_Generic
      (T => R.Ring,
       "*" => R."*",
       "/" => R."/",
       FromInt => R.FromInt,
       ToInt => R.ToInt,
       Power => DH.Power,
       GetGenerator => DH.GetGenerator,
       Modulo => 1234567891);

    Alice, Bob : U.User;

    A, B, M, Enc, Dec : R.Ring;

begin

    Alice := U.Create;
    Bob := U.Create;

    Put_Line("=== DEBUG INFO ===");

    Put_Line("Alice secret: " & Natural'Image(U.GetSecret(Alice)));
    Put_Line("Bob   secret: " & Natural'Image(U.GetSecret(Bob)));

    Put_Line("Generator: " & R.ToString(DH.GetGenerator));

    A := U.GetPublicKey(Alice);
    B := U.GetPublicKey(Bob);

    Put_Line("Alice public key: " & R.ToString(A));
    Put_Line("Bob   public key: " & R.ToString(B));

    U.SetKey(Alice, B);
    U.SetKey(Bob, A);

    Put_Line("Alice shared key: " & R.ToString(U.GetKey(Alice)));
    Put_Line("Bob   shared key: " & R.ToString(U.GetKey(Bob)));

    M := R.FromInt(12345);

    Enc := U.Encrypt(Alice, M);
    Dec := U.Decrypt(Bob, Enc);

    Put_Line("Original:  " & R.ToString(M));
    Put_Line("Encrypted: " & R.ToString(Enc));
    Put_Line("Decrypted: " & R.ToString(Dec));

    if R."="(M, Dec) then
        Put_Line("SUCCESS");
    else
        Put_Line("FAILURE");
    end if;

end Main;