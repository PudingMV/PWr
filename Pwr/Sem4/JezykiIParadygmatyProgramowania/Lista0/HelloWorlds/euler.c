#include <stdio.h>
#include "euler.h"

int NWD(int a, int b)
{
    if(a < b)
    {
        int temp = b;
        b = a;
        a = temp;
    }    

    while(b!=0)
    {
        int temp = b;
        b = a&b;
        a = temp;
    }

    return a;
}

int minimalDivisor(int n)
{
    if(n%2 == 0)
    {
        return 2;
    }

    for(int i=3; i*i<=n;i+=2)
    {
        if(n%i == 0)
        {
            return i;
        }
    }

    return -1;
}

int Totient(int n)
{
    int ans = 1;
    for(int i=2;i<n;i++)
    {
        if(NWD(i, n) == 1)
        {
            ans++;
        }
    }

    return ans;
}

Pair diophantic(int a, int b)
{
    Pair ans;

    int c = NWD(a,b);

    for(int k=-100000; k<=100000; k++)
    {
        int xn = a + k * (b / c);
        int yn = b + k * (a / c);

        if(xn > 0 && yn > 0)
        {
            ans.first = xn;
            ans.second = yn;
            return ans;
        }
    }
}