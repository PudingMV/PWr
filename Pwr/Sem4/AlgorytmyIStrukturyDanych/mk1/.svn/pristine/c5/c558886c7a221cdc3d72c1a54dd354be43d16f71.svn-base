#include <stdio.h>
#include <stdlib.h>

int **mat;
int m, n;

long long comparisons = 0;

void read_matrix() {
    scanf("%d %d", &m, &n);

    mat = malloc(m * sizeof(int*));

    for (int i = 0; i < m; i++) {
        mat[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
}

void free_matrix() {
    for (int i = 0; i < m; i++)
        free(mat[i]);
    free(mat);
}

int search(int x) {

    int i = 0;
    int j = n - 1;

    while (i < m && j >= 0) {

        comparisons++;

        if (mat[i][j] == x) {
            return 1;
        }

        if (mat[i][j] > x) {
            j--;
        } else {
            i++;
        }
    }

    return 0;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s x\n", argv[0]);
        return 1;
    }

    int x = atoi(argv[1]);

    read_matrix();

    int found = search(x);

    printf("%d %d %lld\n", found, m*n, comparisons);

    free_matrix();

    return 0;
}