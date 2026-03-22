#include <stdio.h>
#include "euler.h"

int main()
{
    printf("%d\n", NWD(36, 8));
    printf("%d\n", minimalDivisor(15));
    printf("%d\n", Totient(11));

    Pair ans = diophantic(15, 21);
    printf("%d %d\n", ans.first, ans.second);

    return 0;
}