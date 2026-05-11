#include <bits/stdc++.h>
#include <chrono>

using namespace std;

struct Stats {
    long long comparisons = 0;
    long long swaps = 0;
};

void printVec(const vector<int>& A, int l, int r) {
    for (int i = l; i <= r; i++)
        cout << A[i] << " ";
    cout << "\n";
}

void insertionSort(vector<int>& A,
                   int l,
                   int r,
                   Stats& st) {

    for (int i = l + 1; i <= r; i++) {

        int key = A[i];
        int j = i - 1;

        while (j >= l) {

            st.comparisons++;

            if (A[j] > key) {
                A[j + 1] = A[j];
                j--;
            }
            else {
                break;
            }
        }

        A[j + 1] = key;
    }
}

int selectAlgo(vector<int>& A,
               int l,
               int r,
               int k,
               int groupSize,
               Stats& st,
               bool verbose);


int medianOfMedians(vector<int>& A,
                    int l,
                    int r,
                    int groupSize,
                    Stats& st,
                    bool verbose) {

    int n = r - l + 1;

    if (verbose) {
        cout << "\nBuilding medians...\n";
    }

    if (n <= groupSize) {

        insertionSort(A, l, r, st);

        return A[l + n / 2];
    }

    int medianIndex = l;

    for (int i = l; i <= r; i += groupSize) {

        int subRight = min(i + groupSize - 1, r);

        insertionSort(A, i, subRight, st);

        int med = A[i + (subRight - i) / 2];

        swap(A[medianIndex], A[i + (subRight - i) / 2]);

        medianIndex++;

        if (verbose) {

            cout << "Group: ";

            printVec(A, i, subRight);

            cout << "Median: " << med << "\n";
        }
    }

    if (verbose) {

        cout << "Collected medians: ";

        printVec(A, l, medianIndex - 1);
    }

    int mid =
        l + (medianIndex - l - 1) / 2;

    return selectAlgo(
        A,
        l,
        medianIndex - 1,
        mid,
        groupSize,
        st,
        verbose
    );
}

int selectAlgo(vector<int>& A,
               int l,
               int r,
               int k,
               int groupSize,
               Stats& st,
               bool verbose) {

    if (l == r)
        return A[l];

    int pivot = medianOfMedians(
        A,
        l,
        r,
        groupSize,
        st,
        verbose
    );

    if (verbose) {
        cout << "\nPivot (MoM): "
             << pivot
             << "\n";
    }

    int lt = l;
    int gt = r;
    int i = l;

    while (i <= gt) {

        st.comparisons++;

        if (A[i] < pivot) {
            st.swaps++;

            swap(A[i], A[lt]);

            i++;
            lt++;
        }
        else {

            st.comparisons++;

            if (A[i] > pivot) {

                st.swaps++;

                swap(A[i], A[gt]);

                gt--;
            }
            else {

                i++;
            }
        }
    }

    if (verbose) {

        cout << "L: ";
        printVec(A, l, lt - 1);

        cout << "E: ";
        printVec(A, lt, gt);

        cout << "R: ";
        printVec(A, gt + 1, r);
    }

    if (k < lt) {

        return selectAlgo(
            A,
            l,
            lt - 1,
            k,
            groupSize,
            st,
            verbose
        );
    }

    else if (k <= gt) {

        return pivot;
    }

    else {

        return selectAlgo(
            A,
            gt + 1,
            r,
            k,
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

        cout << "Group size: "
             << groupSize
             << "\n";

        cout << "Initial array: ";

        printVec(A, 0, n - 1);
    }

    auto start =
        chrono::high_resolution_clock::now();

    int result = selectAlgo(
        A,
        0,
        n - 1,
        k - 1,
        groupSize,
        st,
        verbose
    );

    auto end =
        chrono::high_resolution_clock::now();

    auto duration =
        chrono::duration_cast<
            chrono::microseconds
        >(end - start);

    if (verbose) {

        cout << "\nFINAL ARRAY: ";

        printVec(A, 0, n - 1);

        cout << "SORTED CHECK: ";

        sort(orig.begin(), orig.end());

        printVec(orig, 0, n - 1);
    }

    cout << "k-th statistic: "
         << result
         << "\n";

    cout << "Comparisons: "
         << st.comparisons
         << "\n";

    cout << "Time(us): "
         << duration.count()
         << "\n";

    return 0;
}