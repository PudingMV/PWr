#include <bits/stdc++.h>
#include <chrono>

using namespace std;

struct Stats {
    long long comparisons = 0;
};

void printVec(const vector<int>& A) {
    for (int x : A)
        cout << x << " ";
    cout << "\n";
}

int selectAlgo(vector<int> A,
    int k,
    int groupSize,
    Stats& st,
    bool verbose);

int medianOfMedians(vector<int> A,
                    int groupSize,
                    Stats& st,
                    bool verbose) {

    if (verbose) {
        cout << "\nBuilding medians...\n";
    }

    if (A.size() <= (size_t)groupSize) {
        sort(A.begin(), A.end(),
    [&](int a, int b) {
        st.comparisons++;
        return a < b;
    });
        return A[A.size() / 2];
    }

    vector<int> medians;

    for (size_t i = 0; i < A.size(); i += groupSize) {

        vector<int> group;

        for (size_t j = i;
             j < i + groupSize && j < A.size();
             j++) {

            group.push_back(A[j]);
        }

        sort(group.begin(), group.end(),
    [&](int a, int b) {
        st.comparisons++;
        return a < b;
    });

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

    return selectAlgo(
        medians,
        medians.size() / 2,
        groupSize,
        st,
        verbose
    );
}

int selectAlgo(vector<int> A,
               int k,
               int groupSize,
               Stats& st,
               bool verbose) {

    if (A.size() == 1)
        return A[0];

    int pivot = medianOfMedians(
        A,
        groupSize,
        st,
        verbose
    );

    if (verbose) {
        cout << "\nPivot (MoM): " << pivot << "\n";
    }

    vector<int> L, E, R;

    for (int x : A) {

        st.comparisons++;

        if (x < pivot)
            L.push_back(x);


        else {
            st.comparisons++;
            
            if (x > pivot)
                R.push_back(x);
            else
                E.push_back(x);
            }
    }

    if (verbose) {
        cout << "L: ";
        printVec(L);

        cout << "E: ";
        printVec(E);

        cout << "R: ";
        printVec(R);
    }

    if (k < (int)L.size()) {

        return selectAlgo(
            L,
            k,
            groupSize,
            st,
            verbose
        );
    }

    else if (k < (int)L.size() + (int)E.size()) {

        return pivot;
    }

    else {

        return selectAlgo(
            R,
            k - L.size() - E.size(),
            groupSize,
            st,
            verbose
        );
    }
}

int main(int argc, char* argv[]) {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int groupSize = 5;

    if (argc >= 2)
        groupSize = atoi(argv[1]);

    int n, k;

    cin >> n >> k;

    vector<int> A(n);

    for (int i = 0; i < n; i++)
        cin >> A[i];

    vector<int> orig = A;

    bool verbose = (n <= 30);

    Stats st;

    if (verbose) {
        cout << "\n=== SELECT (Median of Medians) ===\n";
        cout << "Group size: " << groupSize << "\n";

        cout << "Initial array: ";
        printVec(orig);
    }

    auto start = chrono::high_resolution_clock::now();

    int result = selectAlgo(
        A,
        k - 1,
        groupSize,
        st,
        verbose
    );

    auto end = chrono::high_resolution_clock::now();

    auto duration =
        chrono::duration_cast<chrono::microseconds>(
            end - start
        );

    if (verbose) {

        cout << "\nFINAL ARRAY: ";
        printVec(A);

        cout << "SORTED CHECK: ";

        sort(orig.begin(), orig.end());

        printVec(orig);
    }

    cout << "k-th statistic: " << result << "\n";

    cout << "Comparisons: "
         << st.comparisons
         << "\n";

    cout << "Time(us): "
         << duration.count()
         << "\n";

    return 0;
}