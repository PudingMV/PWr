with Ada.Text_IO; use Ada.Text_IO;

with Ring_Generic;
with RSA_Generic;

procedure Main is

    package My_Ring is new Ring_Generic(10007 * 10009);
    use My_Ring;
    package My_RSA  is new RSA_Generic(My_Ring);

    R : My_RSA.RSA;

    M, C, D : My_Ring.Ring;

begin
    My_RSA.Init(R, 10007, 10009);

    M := My_Ring.FromInt(1234);

    C := My_RSA.Encrypt(R, M);
    D := My_RSA.Decrypt(R, C);

    Put_Line("Modulo: " & Integer'Image(My_RSA.GetModulo(R)));
    Put_Line("Public key: " & Integer'Image(My_RSA.GetPublicKey(R)));

    Put_Line("Message: " & My_Ring.ToString(M));
    Put_Line("Encrypted: " & My_Ring.ToString(C));
    Put_Line("Decrypted: " & My_Ring.ToString(D));

    if M = D then
        Put_Line("OK");
    else
        Put_Line("BLAD");
    end if;

end Main;