#include <bits/stdc++.h>
using namespace std;

struct Stats {
    long long comparisons = 0;
    long long swaps = 0;     // real swaps
    long long moves = 0;     // logical data movements
    long long rec_calls = 0;
};

int GROUP_SIZE = 5;

// ------------------ swap ------------------

inline void doSwap(int &a, int &b, Stats &st) {
    swap(a, b);
    st.swaps++;
    st.moves += 3; // klasyczny koszt swap (3 operacje)
}

// ------------------ partition ------------------

int partition(vector<int> &A, int l, int r, int pivot, Stats &st) {
    int i = l;

    for (int j = l; j <= r; j++) {
        st.comparisons++;

        if (A[j] < pivot) {
            doSwap(A[i], A[j], st);
            i++;
        }
    }

    return i;
}

// ------------------ median of medians ------------------

int medianOfMedians(vector<int> &A, int l, int r, Stats &st) {
    int n = r - l + 1;

    vector<int> medians;
    medians.reserve((n + GROUP_SIZE - 1) / GROUP_SIZE);

    for (int i = l; i <= r; i += GROUP_SIZE) {
        int rr = min(i + GROUP_SIZE - 1, r);

        vector<int> group;
        group.reserve(rr - i + 1);

        for (int j = i; j <= rr; j++) {
            group.push_back(A[j]);
            st.moves++;
        }

        sort(group.begin(), group.end());

        medians.push_back(group[group.size() / 2]);
        st.moves++;
    }

    // ważne: NIE rekurencja SELECT → tylko stabilna mediana
    sort(medians.begin(), medians.end());
    return medians[medians.size() / 2];
}

// ------------------ SELECT ------------------

int selectAlgo(vector<int> &A, int l, int r, int k, Stats &st) {
    st.rec_calls++;

    if (l == r) return A[l];

    int pivot = medianOfMedians(A, l, r, st);

    int p = partition(A, l, r, pivot, st);

    int leftSize = p - l;

    if (k < leftSize) {
        return selectAlgo(A, l, p - 1, k, st);
    } 
    else if (k < leftSize + (r - p + 1)) {
        return pivot;
    } 
    else {
        return selectAlgo(A, p, r, k - leftSize, st);
    }
}

// ------------------ MAIN ------------------

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc >= 2) {
        GROUP_SIZE = atoi(argv[1]);
    }

    int n, k;
    cin >> n >> k;

    vector<int> A(n);
    for (int i = 0; i < n; i++) cin >> A[i];

    Stats st;

    auto start = chrono::high_resolution_clock::now();

    int result = selectAlgo(A, 0, n - 1, k - 1, st);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_ms = end - start;

    cout << "k-th statistic: " << result << "\n";
    cout << "Comparisons: " << st.comparisons << "\n";
    cout << "Swaps: " << st.swaps << "\n";
    cout << "Moves: " << st.moves << "\n";
    cout << "Recursive_calls: " << st.rec_calls << "\n";
    cout << "Time(ms): " << time_ms.count() << "\n";

    return 0;
}