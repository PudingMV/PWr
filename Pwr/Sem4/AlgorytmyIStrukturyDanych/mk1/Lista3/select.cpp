#include <bits/stdc++.h>
using namespace std;

struct Stats {
    long long comparisons = 0;
};

void printVec(const vector<int>& A) {
    for (int x : A) cout << x << " ";
    cout << "\n";
}

int selectAlgo(vector<int> A, int k, Stats& st, bool verbose);

int medianOfMedians(vector<int> A, Stats& st, bool verbose) {
    if (verbose) {
        cout << "\nBuilding medians...\n";
    }

    if (A.size() <= 5) {
        sort(A.begin(), A.end());
        return A[A.size() / 2];
    }

    vector<int> medians;

    for (size_t i = 0; i < A.size(); i += 5) {
        vector<int> group;

        for (size_t j = i; j < i + 5 && j < A.size(); j++)
            group.push_back(A[j]);

        sort(group.begin(), group.end());

        int med = group[group.size() / 2];
        medians.push_back(med);

        if (verbose) {
            cout << "Group: ";
            printVec(group);
            cout << "Median: " << med << "\n";
        }
    }

    if (verbose) {
        cout << "Medians: ";
        printVec(medians);
    }

    return selectAlgo(medians, medians.size() / 2, st, verbose);
}

int selectAlgo(vector<int> A, int k, Stats& st, bool verbose) {
    if (A.size() == 1) return A[0];

    int pivot = medianOfMedians(A, st, verbose);

    if (verbose) {
        cout << "\nPivot (MoM): " << pivot << "\n";
    }

    vector<int> L, R, E;

    for (int x : A) {
        st.comparisons++;
        if (x < pivot) L.push_back(x);
        else if (x > pivot) R.push_back(x);
        else E.push_back(x);
    }

    if (verbose) {
        cout << "L: "; printVec(L);
        cout << "E: "; printVec(E);
        cout << "R: "; printVec(R);
    }

    if (k < (int)L.size())
        return selectAlgo(L, k, st, verbose);
    else if (k < (int)L.size() + (int)E.size())
        return pivot;
    else
        return selectAlgo(R, k - L.size() - E.size(), st, verbose);
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

    cout << "\n=== SELECT (MEDIAN OF MEDIANS) ===\n";
    cout << "Initial: ";
    printVec(orig);

    int result = selectAlgo(A, k - 1, st, verbose);

    cout << "\nFINAL ARRAY: ";
    printVec(A);

    cout << "SORTED CHECK: ";
    sort(orig.begin(), orig.end());
    printVec(orig);

    cout << "k-th statistic: " << result << "\n";
    cout << "Comparisons: " << st.comparisons << "\n";
}