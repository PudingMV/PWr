#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    int n = atoi(argv[1]);

    srand(time(NULL));

    printf("%d\n", n);

    for (int i=0;i<n;i++)
        printf("%d ", rand() % (2*n));

    return 0;
}