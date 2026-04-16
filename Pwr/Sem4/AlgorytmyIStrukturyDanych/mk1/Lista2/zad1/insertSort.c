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

void insertion_sort(int arr[], int n, int show) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && compare(arr[j], key) > 0) {
            arr[j+1] = arr[j];
            swaps++;
            j--;
        }
        arr[j+1] = key;
        swaps++;

        if (DEBUG && show) print_array(arr, n);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) DEBUG = atoi(argv[1]);

    int n;
    scanf("%d", &n);

    int *arr = malloc(n*sizeof(int));
    int *orig = malloc(n*sizeof(int));

    for (int i=0;i<n;i++){
        scanf("%d",&arr[i]);
        orig[i]=arr[i];
    }

    int show = (n < 40);

    if (DEBUG && show) {
        printf("Wejście:\n");
        print_array(arr,n);
    }

    insertion_sort(arr,n,show);

    if (DEBUG && show) {
        printf("Po sortowaniu:\n");
        print_array(arr,n);
    }

    if (DEBUG) {
        printf("Porównania: %lld\n", comparisons);
        printf("Przestawienia: %lld\n", swaps);
        printf("Poprawność: %s\n", is_sorted(arr,n)?"OK":"BŁĄD");
    } else {
        printf("%d %lld %lld\n", n, comparisons, swaps);
    }

    free(arr); free(orig);
}