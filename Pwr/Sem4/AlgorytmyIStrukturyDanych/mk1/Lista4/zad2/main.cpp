#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

#include "Tree.hpp"

using namespace std;

struct Aggregate
{
    long long compSum = 0, compMax = 0;
    long long pointerOpSum = 0, pointerOpMax = 0;
    long long heightSum = 0, heightMax = 0;

    long long ops = 0;
};

void resetAgg(Aggregate& a)
{
    a = Aggregate{};
}

void updateAgg(Aggregate& a, Tree& t)
{
    auto& s = t.currentStats;

    a.compSum += s.comparisons;
    a.pointerOpSum += s.pointerOperations;

    a.compMax = max(a.compMax, s.comparisons);
    a.pointerOpMax = max(a.pointerOpMax, s.pointerOperations);
    

    long long h = t.height();

    a.heightSum += h;
    a.heightMax = max(a.heightMax, h);

    a.ops++;
}

vector<int> makeSequential(int n)
{
    vector<int> v(n);
    for (int i = 0; i < n; i++)
        v[i] = i + 1;
    return v;
}

int main()
{
    random_device rd;
    mt19937 gen(rd());

    ofstream file("results.csv");

    file << "scenario,n,"
         << "avgComp,maxComp,"
         << "avgPointerOps,maxPointerOps,"
         << "avgHeight,maxHeight\n";

    const int TRIALS = 20;

    for (int scenario = 1; scenario <= 2; scenario++)
    {
        for (int n = 10000; n <= 100000; n += 10000)
        {
            Aggregate agg;

            for (int t = 0; t < TRIALS; t++)
            {
                Tree tree;

                vector<int> values = makeSequential(n);

                if(scenario == 2)
                {
                    shuffle(values.begin(), values.end(), gen);
                }

                for (int x : values)
                {
                    tree.insert(x);
                    updateAgg(agg, tree);
                }

                shuffle(values.begin(), values.end(), gen);

                for (int x : values)
                {
                    tree.deleteValue(x);
                    updateAgg(agg, tree);
                }
            }

            file << scenario << ","
                 << n << ","

                 << (double)agg.compSum / agg.ops << ","
                 << agg.compMax << ","

                << (double)agg.pointerOpSum / agg.ops << ","
                << agg.pointerOpMax << ","

                 << (double)agg.heightSum / agg.ops << ","
                 << agg.heightMax
                 << "\n";

            cout << "Done scenario " << scenario
                 << " n=" << n << "\n";
        }
    }

    file.close();
    return 0;
}