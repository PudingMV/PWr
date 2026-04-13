#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

long long comparisons = 0;
long long swaps = 0;
int DEBUG = 1;

int compare(int a, int b) {
    comparisons++;
    return a - b;
}

void swap(int *a, int *b) {
    swaps++;
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%02d ", arr[i]);
    printf("\n");
}

bool is_sorted(int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i-1] > arr[i]) return false;
    return true;
}

int THRESHOLD = 10;

void insertion_range(int arr[], int l, int r, int n, int show) {
    for (int i = l + 1; i <= r; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= l && compare(arr[j], key) > 0) {
            arr[j+1] = arr[j];
            swaps++;
            j--;
        }
        arr[j+1] = key;
        swaps++;

        if (DEBUG && show)
            print_array(arr, n);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low-1;

    for (int j=low;j<high;j++) {
        if (compare(arr[j], pivot) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i+1;
}

void hybrid_sort(int arr[], int low, int high, int n, int show) {

    if (high - low + 1 <= THRESHOLD) {
        insertion_range(arr, low, high, n, show);
        return;
    }

    if (low < high) {
        int pi = partition(arr, low, high);

        if (DEBUG && show)
            print_array(arr, n);

        hybrid_sort(arr, low, pi - 1, n, show);
        hybrid_sort(arr, pi + 1, high, n, show);
    }
}

int main(int argc,char *argv[]) {
    if (argc > 1) DEBUG = atoi(argv[1]);
    if (argc > 2) THRESHOLD = atoi(argv[2]);

    int n; scanf("%d",&n);

    int *arr = malloc(n*sizeof(int));

    for(int i=0;i<n;i++) scanf("%d",&arr[i]);

    int show = (n < 40);

    hybrid_sort(arr,0,n-1,n,show);

    if (DEBUG) {
        printf("Porównania: %lld\n", comparisons);
        printf("Przestawienia: %lld\n", swaps);
    } else {
        printf("%d %lld %lld\n", n, comparisons, swaps);
    }

    free(arr);
}