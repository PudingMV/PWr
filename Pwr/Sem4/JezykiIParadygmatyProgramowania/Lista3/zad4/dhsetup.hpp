#ifndef DHSETUP_HPP
#define DHSETUP_HPP

#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>

template <typename T>
class DHSetup {
    T generator;
    std::mt19937 gen;

public:
    DHSetup() : gen(static_cast<unsigned>(time(nullptr))) {
        generator = findGenerator();
    }

    T getGenerator() const {
        return generator;
    }

    T power(T a, unsigned long b) const {
        T result(1);
        T base = a;

        while (b > 0) {
            if (b & 1)
                result = result * base;
            base = base * base;
            b >>= 1;
        }

        return result;
    }

private:

    std::vector<unsigned long> primeFactors(unsigned long n) const {
        std::vector<unsigned long> factors;

        for (unsigned long i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                factors.push_back(i);
                while (n % i == 0)
                    n /= i;
            }
        }

        if (n > 1)
            factors.push_back(n);

        return factors;
    }

    bool isGenerator(const T& a) const {
        unsigned long pMinus1 = T::mod - 1;
        auto factors = primeFactors(pMinus1);

        for (auto q : factors) {
            if (power(a, pMinus1 / q).get() == 1)
                return false;
        }

        return true;
    }

    T randomElement() {
        std::uniform_int_distribution<unsigned long long> dist(2, T::mod - 2);
        return T(dist(gen));
    }

    T findGenerator() {
        for (int i = 0; i < 10000; ++i) {
            T candidate = randomElement();
            if (isGenerator(candidate))
                return candidate;
        }

        throw std::runtime_error("No generator found");
    }
};

#endif