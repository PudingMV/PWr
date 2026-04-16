#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);

    printf("%d\n", n);

    srand(time(NULL));

    for (int i = 0; i < n; i++)
        printf("%d ", rand() % (2*n));

    printf("\n");
}