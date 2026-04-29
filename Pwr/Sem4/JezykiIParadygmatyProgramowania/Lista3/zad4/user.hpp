#ifndef USER_HPP
#define USER_HPP

#include <random>
#include <stdexcept>

template <typename T>
class User {
    unsigned long long secret;
    T key;
    bool keySet = false;
    DHSetup<T>& setup;

unsigned long randomSecret() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());

    std::uniform_int_distribution<unsigned long long> dist(2, T::mod - 2);
    return dist(gen);
}

public:
    User(DHSetup<T>& dh) : setup(dh) {
        secret = randomSecret();
    }

    T getPublicKey() const {
        return setup.power(setup.getGenerator(), secret);
    }

    void setKey(T a) {
        key = setup.power(a, secret);
        keySet = true;
    }

    T encrypt(T m) {
        if (!keySet) throw std::runtime_error("no key");
        return m * key;
    }

    T decrypt(T c) {
        if (!keySet) throw std::runtime_error("no key");
        return c / key;
    }
};

#endif