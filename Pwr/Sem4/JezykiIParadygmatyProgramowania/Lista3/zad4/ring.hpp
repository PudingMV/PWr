#ifndef RING_HPP
#define RING_HPP

#include <iostream>
#include <stdexcept>


template<unsigned int n>
class Ring
{
    
    long long value;
    
    public:
    static constexpr unsigned int mod = n;
    static_assert(n > 0, "Modulo must be positive");

    explicit Ring(int v = 0) : value((v % n + n) % n) {}
    long long get() const { return value; }

    //mathematical operation
    Ring operator+(const Ring& other) const 
    {
        return Ring(value + other.value);
    }
    Ring operator-(const Ring& other) const 
    {
        return Ring(value - other.value);
    }
    Ring operator*(const Ring& other) const 
    {
        long long tmp = (value * other.value) % n;
        return Ring(tmp);
    }
    Ring operator/(const Ring& other) const 
    {
        long long inv = mod_inverse(other.value);
        return Ring((value % n) * inv % n);
    }

    //substitution oprands
    Ring& operator+=(const Ring& other)
    {
        value = (value + other.value) % n;
        return *this;
    }
    Ring& operator-=(const Ring& other)
    {
        value = (value - other.value + n) % n;
        return *this;
    }
    Ring& operator*=(const Ring& other)
    {
        value = (value * other.value) % n;
        return *this;
    }
    Ring& operator/=(const Ring& other)
    {
        value = (value * mod_inverse(other.value)) % n;
        return *this;
    }

    //comparing operations
    bool operator==(const Ring& other) const {
        return value == other.value;
    }

    bool operator!=(const Ring& other) const {
        return value != other.value;
    }

    bool operator<(const Ring& other) const {
        return value < other.value;
    }

    bool operator>(const Ring& other) const {
        return value > other.value;
    }

    bool operator<=(const Ring& other) const {
        return value <= other.value;
    }

    bool operator>=(const Ring& other) const {
        return value >= other.value;
    }

    //streams
    friend std::ostream& operator<<(std::ostream& os, const Ring& r)
    {
        return os << r.value;
    }
    friend std::istream& operator>>(std::istream& is, Ring& r)
    {
        int v;
        is >> v;
        r = Ring(v);
        return is;
    }

    //int conversion for convinience
    Ring& operator+=(int v) { return *this += Ring(v); }
    Ring& operator-=(int v) { return *this -= Ring(v); }
    Ring& operator*=(int v) { return *this *= Ring(v); }
    Ring& operator/=(int v) { return *this /= Ring(v); }
    Ring& operator=(int v)
    {
        value = (v % n + n) % n;
        return *this;
    }

    private:
    long long mod_inverse(int a) const {
    long long t = 0, new_t = 1;
    long long r = n, new_r = a;

    while (new_r != 0) {
        long long q = r / new_r;

        long long tmp_t = t - q * new_t;
        t = new_t;
        new_t = tmp_t;

        long long tmp_r = r - q * new_r;
        r = new_r;
        new_r = tmp_r;
    }

    if (r != 1)
        throw std::runtime_error("No inverse exists");

    if (t < 0)
        t += n;

    return t;
}
};

#endif