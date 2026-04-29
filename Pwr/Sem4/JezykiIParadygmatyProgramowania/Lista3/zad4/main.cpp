#include <iostream>

#include "Ring.hpp"
#include "DHSetup.hpp"
#include "User.hpp"

int main() {
    try {
        using Field = Ring<1234567891>;

        DHSetup<Field> dh;

        User<Field> alice(dh);
        User<Field> bob(dh);

        Field A = alice.getPublicKey();
        Field B = bob.getPublicKey();

        alice.setKey(B);
        bob.setKey(A);

        std::cout << "Public key Alice: " << A << "\n";
        std::cout << "Public key Bob:   " << B << "\n";

        Field message(12345);

        Field encrypted = alice.encrypt(message);
        Field decrypted = bob.decrypt(encrypted);

        std::cout << "Original:  " << message << "\n";
        std::cout << "Encrypted: " << encrypted << "\n";
        std::cout << "Decrypted: " << decrypted << "\n";

        if (message == decrypted)
            std::cout << "SUCCESS: Diffie-Hellman works!\n";
        else
            std::cout << "FAILURE!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}