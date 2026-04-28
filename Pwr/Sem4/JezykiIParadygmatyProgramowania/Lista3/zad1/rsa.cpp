#include <iostream>
#include "Ring.hpp"
#include "RSA.hpp"

int main()
{
    const unsigned int p = 10007;
    const unsigned int q = 10009;
    const unsigned int n = p * q;

    RSA<Ring<n>> rsa(p, q);

    Ring<n> message(1234);

    auto encrypted = rsa.encrypt(message);
    auto decrypted = rsa.decrypt(encrypted);

    std::cout << "Modulo n: " << rsa.getModulo() << std::endl;
    std::cout << "Public key: " << rsa.getPublicKey() << std::endl;

    std::cout << "Message: " << message << std::endl;
    std::cout << "Encrypted: " << encrypted << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;

    if (message == decrypted)
        std::cout << "OK: deszyfrowanie działa poprawnie\n";
    else
        std::cout << "BLAD: cos poszlo nie tak\n";

    return 0;
}