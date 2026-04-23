#include <bits/stdc++.h>
using namespace std;

const int N = 10;

string fileNames[N] = {
    "wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp",
    "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"
};

struct City {
    int id;
    double x, y;
};


vector<City> readCountry(const string &fileName)
{
    ifstream file(fileName);
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

        if (readCoords)
        {
            stringstream ss(line);
            City c;
            char comma;

            ss >> c.id >> comma >> c.x >> comma >> c.y;
            cities.push_back(c);
        }
    }

    return cities;
}


int Distance(const City &a, const City &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (int)round(sqrt(dx * dx + dy * dy));
}

vector<vector<int>> calculateDistances(const vector<City> &cities)
{
    int n = cities.size();
    vector<vector<int>> dist(n, vector<int>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dist[i][j] = Distance(cities[i], cities[j]);

    return dist;
}


int calculateRoute(const vector<vector<int>> &dist,
                   const vector<int> &perm)
{
    int n = perm.size();
    int sum = 0;

    for (int i = 1; i < n; i++)
        sum += dist[perm[i - 1] - 1][perm[i] - 1];

    sum += dist[perm[n - 1] - 1][perm[0] - 1];

    return sum;
}


void randomPermutate(vector<int> &arr)
{
    for (int i = arr.size() - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
}


pair<int, vector<int>> LocalSearch(
    const vector<vector<int>> &dist,
    int &steps)
{
    int n = dist.size();

    vector<int> current(n);
    for (int i = 0; i < n; i++)
        current[i] = i + 1;

    randomPermutate(current);

    int currentCost = calculateRoute(dist, current);

    steps = 0;

    while (true)
    {
        vector<int> bestNeighbour = current;
        int bestCost = currentCost;

        bool improved = false;

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                vector<int> candidate = current;

                reverse(candidate.begin() + i, candidate.begin() + j + 1);

                int cost = calculateRoute(dist, candidate);

                if (cost < bestCost)
                {
                    bestCost = cost;
                    bestNeighbour = candidate;
                    improved = true;
                }
            }
        }

        if (!improved)
            break;

        current = bestNeighbour;
        currentCost = bestCost;
        steps++;
    }

    return {currentCost, current};
}


void Solve()
{
    srand(time(0));

    for (int f = 0; f < N; f++)
    {
        string path = "data/" + fileNames[f];
        vector<City> cities = readCountry(path);

        int n = cities.size();
        auto dist = calculateDistances(cities);

        double sum = 0;
        double stepsSum = 0;
        int bestCost = INT_MAX;
        vector<int> bestPath;

        for (int k = 0; k < n; k++)
        {
            int steps = 0;

            auto [cost, route] = LocalSearch(dist, steps);

            sum += cost;
            stepsSum += steps;

            if (cost < bestCost)
            {
                bestCost = cost;
                bestPath = route;
            }
        }

        ofstream out("task1_stats_" + fileNames[f]);

        out << fileNames[f] << "\n";
        out << "avg cost: " << sum / n << "\n";
        out << "avg steps: " << stepsSum / n << "\n";
        out << "best: " << bestCost << "\n\n";

        cout << fileNames[f] << "\n";
        cout << "avg cost: " << sum / n << "\n";
        cout << "avg steps: " << stepsSum / n << "\n";
        cout << "best: " << bestCost << "\n\n";

        for (int v : bestPath)
            out << v << " ";
        out << "\n";

    }
}


int main()
{
    Solve();
}