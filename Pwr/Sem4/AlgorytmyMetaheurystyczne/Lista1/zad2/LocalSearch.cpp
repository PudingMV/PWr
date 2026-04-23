#include <bits/stdc++.h>
using namespace std;

const int N = 10;

string fileNames[N] = {
    "wi29.tsp","dj38.tsp","qa194.tsp","uy734.tsp","zi929.tsp",
    "mu1979.tsp","ca4663.tsp","tz6117.tsp","eg7146.tsp","ei8246.tsp"
};

struct City {
    int id;
    double x, y;
};

vector<City> readCountry(const string &fileName)
{
    ifstream file(fileName);
    string line;
    vector<City> cities;

    bool readCoords = false;

    while (getline(file, line))
    {
        if (line.find("NODE_COORD_SECTION") != string::npos)
        {
            readCoords = true;
            continue;
        }

        if (line.find("EOF") != string::npos) break;

        if (readCoords)
        {
            stringstream ss(line);
            City c;
            ss >> c.id >> c.x >> c.y;
            cities.push_back(c);
        }
    }

    return cities;
}

int dist(const City &a, const City &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (int)round(sqrt(dx*dx + dy*dy));
}

vector<vector<int>> buildDist(const vector<City> &cities)
{
    int n = cities.size();
    vector<vector<int>> d(n, vector<int>(n));

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            d[i][j] = dist(cities[i], cities[j]);

    return d;
}

int routeCost(const vector<vector<int>> &d, const vector<int> &p)
{
    int n = p.size();
    int sum = 0;

    for(int i = 1; i < n; i++)
        sum += d[p[i-1]][p[i]];

    sum += d[p[n-1]][p[0]];
    return sum;
}

void shuffleVec(vector<int> &v)
{
    for(int i = v.size() - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(v[i], v[j]);
    }
}

pair<int, vector<int>> LocalSearchRandom(
    const vector<City> &cities,
    int &steps
)
{
    auto d = buildDist(cities);
    int n = cities.size();

    vector<int> current(n);
    iota(current.begin(), current.end(), 0);
    shuffleVec(current);

    vector<int> best = current;
    int bestCost = routeCost(d, current);

    steps = 0;

    while(true)
    {
        vector<int> bestNeighbour = current;
        int bestNeighbourCost = bestCost;

        bool improved = false;

        for(int k = 0; k < n; k++)
        {
            int i = rand() % n;
            int j = rand() % n;
            if(i > j) swap(i, j);
            if(i == j) continue;

            vector<int> candidate = current;
            reverse(candidate.begin() + i, candidate.begin() + j + 1);

            int c = routeCost(d, candidate);

            if(c < bestNeighbourCost)
            {
                bestNeighbourCost = c;
                bestNeighbour = candidate;
                improved = true;
            }
        }

        if(!improved)
            break;

        current = bestNeighbour;

        if(bestNeighbourCost < bestCost)
        {
            bestCost = bestNeighbourCost;
            best = current;
        }

        steps++;
    }

    return {bestCost, best};
}

void Solve()
{
    for(int f = 0; f < N; f++)
    {
        vector<City> cities = readCountry("data/" + fileNames[f]);
        int n = cities.size();

        double sumCost = 0;
        double sumSteps = 0;

        int bestCost = INT_MAX;
        vector<int> bestPath;

        for(int run = 0; run < n; run++)
        {
            int steps = 0;
            auto res = LocalSearchRandom(cities, steps);

            sumCost += res.first;
            sumSteps += steps;

            if(res.first < bestCost)
            {
                bestCost = res.first;
                bestPath = res.second;
            }
        }

        ofstream out("task2_stats_" + fileNames[f]);

        out << fileNames[f] << "\n";
        out << "avg cost: " << sumCost / n << "\n";
        out << "avg steps: " << sumSteps / n << "\n";
        out << "best: " << bestCost << "\n\n";

        for(int v : bestPath)
            out << v << " ";

        out << "\n";

        cout << fileNames[f] << "\n";
        cout << "avg cost: " << sumCost / n << "\n";
        cout << "avg steps: " << sumSteps / n << "\n";
        cout << "best: " << bestCost << "\n\n";
    }
}

int main()
{
    srand(time(0));
    Solve();
}