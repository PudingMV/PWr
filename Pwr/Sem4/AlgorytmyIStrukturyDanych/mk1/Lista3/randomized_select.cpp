#include <bits/stdc++.h>
using namespace std;

struct Stats {
    long long comparisons = 0;
    long long swaps = 0;
};

void printVec(const vector<int>& A) {
    for (int x : A) cout << x << " ";
    cout << "\n";
}

void swapVec(vector<int>& A, int i, int j, Stats& st) {
    if (i != j) {
        swap(A[i], A[j]);
        st.swaps++;
    }
}

int partition(vector<int>& A, int p, int r, Stats& st, bool verbose) {
    int x = A[r];
    int i = p - 1;

    if (verbose) {
        cout << "\nPartition, pivot = " << x << "\n";
    }

    for (int j = p; j < r; j++) {
        st.comparisons++;
        if (A[j] <= x) {
            i++;
            swapVec(A, i, j, st);
            if (verbose) {
                cout << "swap: ";
                printVec(A);
            }
        }
    }

    swapVec(A, i + 1, r, st);
    return i + 1;
}

int randomizedPartition(vector<int>& A, int p, int r, Stats& st, bool verbose) {
    int i = p + rand() % (r - p + 1);

    if (verbose) {
        cout << "\nRandom pivot index: " << i << " value: " << A[i] << "\n";
    }

    swapVec(A, i, r, st);
    return partition(A, p, r, st, verbose);
}

int randomizedSelect(vector<int>& A, int p, int r, int k, Stats& st, bool verbose) {
    if (p == r) return A[p];

    int q = randomizedPartition(A, p, r, st, verbose);
    int i = q - p + 1;

    if (verbose) {
        cout << "After partition: ";
        printVec(A);
    }

    if (k == i) return A[q];
    else if (k < i) return randomizedSelect(A, p, q - 1, k, st, verbose);
    else return randomizedSelect(A, q + 1, r, k - i, st, verbose);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> A(n);
    for (int i = 0; i < n; i++) cin >> A[i];

    vector<int> orig = A;
    bool verbose = n <= 30;

    Stats st;
    srand(time(NULL));

    cout << "\n=== RANDOMIZED SELECT ===\n";
    cout << "Initial: ";
    printVec(orig);

    int result = randomizedSelect(A, 0, n - 1, k, st, verbose);

    cout << "\nFINAL ARRAY: ";
    printVec(A);

    cout << "SORTED CHECK: ";
    sort(orig.begin(), orig.end());
    printVec(orig);

    cout << "k-th statistic: " << result << "\n";
    cout << "Comparisons: " << st.comparisons << "\n";
    cout << "Swaps: " << st.swaps << "\n";
}