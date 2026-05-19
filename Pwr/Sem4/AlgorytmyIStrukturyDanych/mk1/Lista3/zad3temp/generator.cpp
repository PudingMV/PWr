#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int n = 10;
    int seed = 12345;

    if (argc >= 2) n = atoi(argv[1]);

    srand(seed + n);

    cout << n << " " << 1 << "\n";

    for (int i = 0; i < n; i++) {
        cout << rand() % (2 * n) << " ";
    }
    cout << "\n";
}