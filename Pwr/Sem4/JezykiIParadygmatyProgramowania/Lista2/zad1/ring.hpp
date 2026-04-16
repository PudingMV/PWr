#ifndef RING_HPP
#define RING_HPP

#include <iostream>
#include <stdexcept>


template<unsigned int n>
class Ring
{
    
    int value;
    
    public:
    static constexpr unsigned int mod = n;
    static_assert(n > 0, "Modulo must be positive");

    explicit Ring(int v = 0) : value((v % n + n) % n) {}
    int get() const {return value;}

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
        return Ring(value * other.value);
    }
    Ring operator/(const Ring& other) const 
    {
        return Ring(value * mod_inverse(other.value));
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
    int mod_inverse(int a) const {
    int t = 0, new_t = 1;
    int r = n, new_r = a;

    while (new_r != 0) {
        int q = r / new_r;

        int tmp_t = t - q * new_t;
        t = new_t;
        new_t = tmp_t;

        int tmp_r = r - q * new_r;
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