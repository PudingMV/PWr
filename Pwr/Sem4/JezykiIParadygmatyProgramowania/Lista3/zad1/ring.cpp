#include <iostream>
#include <stdexcept>
#include "ring.hpp"

using namespace std;

int main()
{
    Ring<5> a(4);
    Ring<5> b(2);

    cout << "a = " << a << ", b = " << b 
         << ", w pierścieniu: " << Ring<5>::mod << "\n\n";

    //mathematical operations
    cout << "Mathematical operations:\n";
    cout << "a + b = " << a + b << "\n";
    cout << "a - b = " << a - b << "\n";
    cout << "a * b = " << a * b << "\n";
    cout << "a / b = " << a / b << "\n\n";

    //assignment operations
    cout << "Assignment operators:\n";

    Ring<5> tmp = a;

    cout << "a += 5: " << tmp << " += 5 ";
    tmp += 5;
    cout << "= " << tmp << "\n";

    tmp = a;
    cout << "a -= 3: " << tmp << " -= 3 ";
    tmp -= 3;
    cout << "= " << tmp << "\n";

    tmp = a;
    cout << "a *= 4: " << tmp << " *= 4 ";
    tmp *= 4;
    cout << "= " << tmp << "\n";

    tmp = a;
    cout << "a /= 2: " << tmp << " /= 2 ";
    tmp /= 2;
    cout << "= " << tmp << "\n\n";

    //comparisons
    cout << "Comparisons:\n";
    cout << "a = " << a << ", b = " << b << "\n";

    cout << "a == b: " << (a == b) << "\n";
    cout << "a != b: " << (a != b) << "\n";
    cout << "a >= b: " << (a >= b) << "\n";
    cout << "a <= b: " << (a <= b) << "\n";
    cout << "a > b: " << (a > b) << "\n";
    cout << "a < b: " << (a < b) << "\n\n";

    //conversions
    cout << "Conversions:\n";
    Ring<5> c;
    c = 12;
    cout << "c = 12 -> " << c << "\n\n";

    //negatives
    cout << "Negative numbers:\n";
    Ring<5> d(-1);
    cout << "Ring(-1) = " << d << " (should be 4)\n\n";

    //exceptions
    cout << "Exception Test (No Inverse):\n";
    try {
        Ring<6> x(2);
        Ring<6> y(3);
        cout << "2 / 3 mod 6 = " << x / y << "\n";
    } catch (const exception& ex) {
        cout << "Exception caught: " << ex.what() << "\n";
    }
    cout << "\n";

    //input
    cout << "Write a number (mod 5): ";
    Ring<5> input;
    cin >> input;
    cout << "Read: " << input << "\n";

    return 0;
}
