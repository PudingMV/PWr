#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    srand(time(NULL));

    int n = 10;
    int k = 1;

    if (argc >= 2) n = atoi(argv[1]);
    if (argc >= 3) k = atoi(argv[2]);

    cout << n << " " << k << "\n";

    for (int i = 0; i < n; i++) {
        int x = rand() % (2 * n);
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}