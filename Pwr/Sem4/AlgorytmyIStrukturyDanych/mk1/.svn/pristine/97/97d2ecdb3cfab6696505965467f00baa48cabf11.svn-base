#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Użycie: %s m n\n", argv[0]);
        return 1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    srand(time(NULL));

    int **mat = malloc(m * sizeof(int*));
    for (int i = 0; i < m; i++)
        mat[i] = malloc(n * sizeof(int));

    int base = rand() % 5;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {

            int from_left = (j > 0) ? mat[i][j-1] : base;
            int from_up   = (i > 0) ? mat[i-1][j] : base;

            int min_prev = (from_left > from_up) ? from_left : from_up;

            mat[i][j] = min_prev + (rand() % 5 + 1);
        }
    }

    printf("%d %d\n", m, n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < m; i++)
        free(mat[i]);
    free(mat);

    return 0;
}