#include <bits/stdc++.h>
using namespace std;

struct Stats {
    long long comparisons = 0;
};

double f(int i) {
    return pow(log2(i + 1), 3.0);
}

// pochodna f
double df(int i) {
    double x = log2(i + 1);
    return (3.0 * x * x) / ((i + 1) * log(2));
}

// Δi ≈ δ / f'(i)
int deltaI(int i, double delta) {
    double deriv = max(df(i), 1e-9);
    return max(10, (int)(delta / deriv));
}

vector<double> generateArray(int n) {
    vector<double> A(n);

    for (int i = 0; i < n; i++) {
        double noise = ((double)rand() / RAND_MAX) * 10.0;
        A[i] = f(i) + noise;
    }

    sort(A.begin(), A.end());
    return A;
}

bool smartSearch(const vector<double>& A, double q, Stats& st, bool verbose) {
    int n = A.size();

    int guess = (int)(pow(2.0, cbrt(q)) - 1);
    guess = min(max(guess, 0), n - 1);

    int d = deltaI(guess, 10.0);

    int L = max(0, guess - d);
    int R = min(n - 1, guess + d);

    if (verbose) {
        cout << "\n[SMART SEARCH]\n";
        cout << "q = " << q << "\n";
        cout << "guess = " << guess << "\n";
        cout << "delta i = " << d << "\n";
        cout << "range = [" << L << ", " << R << "]\n";
    }

    int steps = 0;

    while (L <= R) {
        int mid = (L + R) / 2;
        st.comparisons++;
        steps++;

        if (verbose && steps <= 5) {
            cout << "step " << steps
                 << " mid=" << mid
                 << " A[mid]=" << A[mid] << "\n";
        }

        if (fabs(A[mid] - q) < 1e-9) {
            if (verbose) cout << "FOUND at " << mid << "\n";
            return true;
        }

        if (A[mid] < q)
            L = mid + 1;
        else
            R = mid - 1;
    }

    if (verbose) cout << "NOT FOUND in window\n";
    return false;
}

bool binarySearch(const vector<double>& A, double q, Stats& st, bool verbose) {
    int L = 0, R = A.size() - 1;

    if (verbose) cout << "\n[BINARY SEARCH]\n";

    int steps = 0;

    while (L <= R) {
        int mid = (L + R) / 2;
        st.comparisons++;
        steps++;

        if (verbose && steps <= 5) {
            cout << "step " << steps
                 << " mid=" << mid
                 << " A[mid]=" << A[mid] << "\n";
        }

        if (fabs(A[mid] - q) < 1e-9) {
            if (verbose) cout << "FOUND at " << mid << "\n";
            return true;
        }

        if (A[mid] < q)
            L = mid + 1;
        else
            R = mid - 1;
    }

    if (verbose) cout << "NOT FOUND\n";
    return false;
}

int main() {
    srand(time(NULL));

    int n;
    cin >> n;

    auto A = generateArray(n);

    double q = A[rand() % n];

    bool verbose = (n <= 30);

    Stats s1, s2;

    smartSearch(A, q, s1, verbose);
    binarySearch(A, q, s2, verbose);

    cout << "\nSMART comparisons: " << s1.comparisons << "\n";
    cout << "BINARY comparisons: " << s2.comparisons << "\n";
}