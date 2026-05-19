#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////
// FILES
//////////////////////////////////////////////////////////////

const int FILES = 10;

string fileNames[FILES] = {
    "wi29.tsp",
    "dj38.tsp",
    "qa194.tsp",
    "uy734.tsp",
    "zi929.tsp",
    "mu1979.tsp",
    "ca4663.tsp",
    "tz6117.tsp",
    "eg7146.tsp",
    "ei8246.tsp"
};

//////////////////////////////////////////////////////////////
// CITY
//////////////////////////////////////////////////////////////

struct City
{
    int id;
    double x, y;
};

//////////////////////////////////////////////////////////////
// RANDOM
//////////////////////////////////////////////////////////////

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//////////////////////////////////////////////////////////////
// READ TSPLIB FILE
//////////////////////////////////////////////////////////////

vector<City> readCountry(const string &fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << "ERROR opening file: "
             << fileName << "\n";

        exit(1);
    }

    vector<City> cities;

    string line;

    bool readCoords = false;

    while (getline(file, line))
    {
        if (line.find("NODE_COORD_SECTION") != string::npos)
        {
            readCoords = true;
            continue;
        }

        if (line.find("EOF") != string::npos)
            break;

        if (!readCoords)
            continue;

        stringstream ss(line);

        City c;

        if (ss >> c.id >> c.x >> c.y)
        {
            cities.push_back(c);
        }
    }

    return cities;
}

//////////////////////////////////////////////////////////////
// DISTANCE
//////////////////////////////////////////////////////////////

inline int Distance(
    const vector<City> &cities,
    int i,
    int j)
{
    double dx = cities[i].x - cities[j].x;
    double dy = cities[i].y - cities[j].y;

    return (int)round(sqrt(dx * dx + dy * dy));
}

//////////////////////////////////////////////////////////////
// ROUTE COST
//////////////////////////////////////////////////////////////

int calculateRoute(
    const vector<City> &cities,
    const vector<int> &perm)
{
    int n = perm.size();

    int sum = 0;

    for (int i = 0; i < n; i++)
    {
        int a = perm[i];
        int b = perm[(i + 1) % n];

        sum += Distance(cities, a, b);
    }

    return sum;
}

//////////////////////////////////////////////////////////////
// RANDOM PERMUTATION
//////////////////////////////////////////////////////////////

void randomPermutate(vector<int> &arr)
{
    shuffle(arr.begin(), arr.end(), rng);
}

//////////////////////////////////////////////////////////////
// LOCAL SEARCH (2-OPT)
//////////////////////////////////////////////////////////////

pair<int, vector<int>> LocalSearch(
    const vector<City> &cities,
    int &steps)
{
    int n = cities.size();

    vector<int> current(n);

    for (int i = 0; i < n; i++)
    {
        current[i] = i;
    }

    randomPermutate(current);

    int currentCost =
        calculateRoute(cities, current);

    steps = 0;

    bool improved = true;

    while (improved)
    {
        improved = false;

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 2; j < n; j++)
            {
                // avoid breaking same edge
                if (i == 0 && j == n - 1)
                    continue;

                int a = current[i];
                int b = current[(i + 1) % n];

                int c = current[j];
                int d = current[(j + 1) % n];

                int oldEdges =
                    Distance(cities, a, b) +
                    Distance(cities, c, d);

                int newEdges =
                    Distance(cities, a, c) +
                    Distance(cities, b, d);

                int delta = newEdges - oldEdges;

                // improvement
                if (delta < 0)
                {
                    reverse(
                        current.begin() + i + 1,
                        current.begin() + j + 1);

                    currentCost += delta;

                    steps++;

                    improved = true;

                    goto next_iteration;
                }
            }
        }

    next_iteration:;
    }

    return {currentCost, current};
}

//////////////////////////////////////////////////////////////
// SOLVER
//////////////////////////////////////////////////////////////

void Solve()
{
    for (int f = 0; f < FILES; f++)
    {
        string path = "data/" + fileNames[f];

        cout << "\n====================================\n";

        cout << "Loading: "
             << path
             << "\n";

        vector<City> cities =
            readCountry(path);

        int n = cities.size();

        cout << "Cities: "
             << n
             << "\n";

        //////////////////////////////////////////////////////////
        // RUN COUNT
        //////////////////////////////////////////////////////////

        int RUNS =
            (n > 1000 ? 100 : n);

        //////////////////////////////////////////////////////////

        double sumCost = 0;
        double sumSteps = 0;

        int bestCost = INT_MAX;

        vector<int> bestPath;

        //////////////////////////////////////////////////////////

        for (int run = 0; run < RUNS; run++)
        {
            int steps = 0;

            auto [cost, route] =
                LocalSearch(cities, steps);

            sumCost += cost;
            sumSteps += steps;

            if (cost < bestCost)
            {
                bestCost = cost;
                bestPath = route;
            }

            cout << "Run "
                 << run + 1
                 << "/"
                 << RUNS
                 << "\r";

            cout.flush();
        }

        cout << "\n";

        //////////////////////////////////////////////////////////
        // OUTPUT FILE
        //////////////////////////////////////////////////////////

        string outName =
            "task1_stats_" + fileNames[f];

        ofstream out(outName);

        out << fileNames[f] << "\n";

        out << "cities: "
            << n
            << "\n";

        out << "runs: "
            << RUNS
            << "\n";

        out << "avg cost: "
            << sumCost / RUNS
            << "\n";

        out << "avg steps: "
            << sumSteps / RUNS
            << "\n";

        out << "best: "
            << bestCost
            << "\n\n";

        out << "Best route:\n";

        for (int v : bestPath)
        {
            out << cities[v].id << " ";
        }

        out << "\n";

        //////////////////////////////////////////////////////////
        // CONSOLE
        //////////////////////////////////////////////////////////

        cout << "Average cost: "
             << sumCost / RUNS
             << "\n";

        cout << "Average steps: "
             << sumSteps / RUNS
             << "\n";

        cout << "Best cost: "
             << bestCost
             << "\n";
    }
}

//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Solve();

    return 0;
}