#ifndef RSA_HPP
#define RSA_HPP

#include <random>
#include <stdexcept>

template<typename T>
class RSA
{
    unsigned int n;
    unsigned int phi;
    unsigned int e;
    unsigned int d;

public:
    RSA(unsigned int p, unsigned int q)
    {
        n = p * q;
        phi = (p - 1) * (q - 1);

        e = find_coprime(phi);

        d = mod_inverse(e, phi);
    }

    unsigned int getModulo() const
    {
        return n;
    }

    T getPublicKey() const
    {
        return T(e);
    }

    T encrypt(T m) const
    {
        return mod_pow(m, e);
    }

    T decrypt(T s) const
    {
        return mod_pow(s, d);
    }

private:
    T mod_pow(T base, unsigned int exp) const
    {
        T result = T(1);
        while (exp > 0)
        {
            if (exp % 2 == 1)
                result *= base;
            base *= base;
            exp /= 2;
        }
        return result;
    }

    unsigned int gcd(unsigned int a, unsigned int b) const
    {
        while (b != 0)
        {
            unsigned int t = b;
            b = a % b;
            a = t;
        }
        return a;
    }

    // znajdź e
    unsigned int find_coprime(unsigned int phi)
    {
        for (unsigned int i = 3; i < phi; ++i)
        {
            if (gcd(i, phi) == 1)
                return i;
        }
        throw std::runtime_error("No coprime found");
    }

    unsigned int mod_inverse(unsigned int a, unsigned int m)
    {
        int t = 0, new_t = 1;
        int r = m, new_r = a;

        while (new_r != 0)
        {
            int q = r / new_r;

            int tmp_t = t - q * new_t;
            t = new_t;
            new_t = tmp_t;

            int tmp_r = r - q * new_r;
            r = new_r;
            new_r = tmp_r;
        }

        if (r != 1)
            throw std::runtime_error("No inverse");

        if (t < 0)
            t += m;

        return t;
    }
};

#endif