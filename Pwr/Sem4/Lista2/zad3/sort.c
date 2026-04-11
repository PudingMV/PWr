#include <stdio.h>
#include <stdlib.h>

long long comparisons = 0;
long long swaps = 0;

int compare(int a, int b) {
    comparisons++;
    return a - b;
}

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) { L[i] = arr[l+i]; swaps++; }
    for (int i = 0; i < n2; i++) { R[i] = arr[m+1+i]; swaps++; }

    int i=0, j=0, k=l;

    while (i<n1 && j<n2) {
        if (compare(L[i], R[j]) <= 0)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
        swaps++;
    }

    while (i<n1) { arr[k++] = L[i++]; swaps++; }
    while (j<n2) { arr[k++] = R[j++]; swaps++; }

    free(L);
    free(R);
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

typedef struct {
    int l, r;
} Run;

int find_runs(int arr[], int n, Run runs[]) {
    int c = 0;
    int i = 0;

    while (i < n) {
        int start = i;
        while (i+1 < n && compare(arr[i], arr[i+1]) <= 0)
            i++;
        runs[c++] = (Run){start, i};
        i++;
    }
    return c;
}

int best_pair(Run runs[], int n) {
    int best = 0;
    int best_len = (runs[0].r - runs[0].l) + (runs[1].r - runs[1].l);

    for (int i = 1; i < n-1; i++) {
        int len = (runs[i].r - runs[i].l) + (runs[i+1].r - runs[i+1].l);
        if (len < best_len) {
            best = i;
            best_len = len;
        }
    }
    return best;
}

void adaptive_sort(int arr[], int n) {
    Run runs[60000];
    int rc = find_runs(arr, n, runs);

    while (rc > 1) {
        int i = best_pair(runs, rc);

        int l = runs[i].l;
        int m = runs[i].r;
        int r = runs[i+1].r;

        merge(arr, l, m, r);

        runs[i].r = r;

        for (int j = i+1; j < rc-1; j++)
            runs[j] = runs[j+1];

        rc--;
    }
}

int main(int argc, char **argv) {

    int mode = atoi(argv[1]);

    int n;
    scanf("%d", &n);

    int *arr = malloc(n*sizeof(int));
    for (int i=0;i<n;i++) scanf("%d",&arr[i]);

    if (mode == 0)
        merge_sort(arr,0,n-1);
    else
        adaptive_sort(arr,n);

    printf("%d %lld %lld\n", n, comparisons, swaps);

    free(arr);
}