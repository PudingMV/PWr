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
        b = a%b;
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

    return n;
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

    for(int y=0; y<=100000; y++)
    {
        int test = c + b*y;

        if(test % a != 0)
            continue;

        int x_candidate = test / a;

        if(x_candidate > 0 && y > 0 && (a*x_candidate - b*y == c))
        {
            if(a*x_candidate - b*y == c)
            {
                ans.first = x_candidate;
                ans.second = y;
                return ans;
            }
        }
    }
    ans.first = -1;
    ans.second = -1;
    return ans;
}