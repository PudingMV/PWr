#include <bits/stdc++.h>
#include "ring.hpp"
using namespace std;
int main()
{
    Ring<5> a(4);
    Ring<5> b(2);

    cout << "a = " << a << ", " << "b = " << b << ", w pierścieniu: " << Ring<5>::mod << "\n\n";

    //mathematical operands
    cout << "a + b = " << a + b << "\n";
    cout << "a - b = " << a - b << "\n";
    cout << "a * b = " << a * b << "\n";
    cout << "a / b = " << a / b << "\n\n";

    //substitute operands
    cout << "a += 5: " << a << " += 5 "; a+=5; cout << "= " << a << "\n";
    cout << "a -= 3: " << a << " -= 3 "; a-=3; cout << "= " << a << "\n";
    cout << "a *= 4: " << a << " *= 4 "; a*=4; cout << "= " << a << "\n";
    cout << "a /= 2: " << a << " /= 2 "; a/=2; cout << "= " << a << "\n\n";

    //compare operands
    cout << "a = " << a << ", " << "b = " << b << "\n";

    cout << "a == b: " << (a == b) << "\n";
    cout << "a != b: " << (a != b) << "\n";
    cout << "a >= b: " << (a >= b) << "\n";
    cout << "a <= b: " << (a <= b) << "\n";
    cout << "a > b: " << (a > b) << "\n";
    cout << "a < b: " << (a < b) << "\n";

    a = 3;

    cout << "\n";

    cout << "a = " << a << ", " << "b = " << b << "\n";

    cout << "a == b: " << (a == b) << "\n";
    cout << "a != b: " << (a != b) << "\n";
    cout << "a >= b: " << (a >= b) << "\n";
    cout << "a <= b: " << (a <= b) << "\n";
    cout << "a > b: " << (a > b) << "\n";
    cout << "a < b: " << (a < b) << "\n";
}