#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////
// CONFIG
//////////////////////////////////////////////////////////////

static constexpr int K_NEIGHBORS = 3000;
static constexpr int RUNS = 1000;

//////////////////////////////////////////////////////////////
// RANDOM
//////////////////////////////////////////////////////////////

mt19937 rng(
    chrono::steady_clock::now()
        .time_since_epoch()
        .count());

//////////////////////////////////////////////////////////////
// CITY
//////////////////////////////////////////////////////////////

struct City
{
    int id;
    double x, y;
};

//////////////////////////////////////////////////////////////
// READ TSPLIB
//////////////////////////////////////////////////////////////

vector<City> readCountry(const string &fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << "Cannot open file: "
             << fileName << "\n";

        exit(1);
    }

    vector<City> cities;

    string line;

    bool readCoords = false;

    while (getline(file, line))
    {
        if (line.find("NODE_COORD_SECTION")
            != string::npos)
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
// DISTANCE MATRIX
//////////////////////////////////////////////////////////////

vector<vector<int>> distMatrix;

inline int Dist(int a, int b)
{
    return distMatrix[a][b];
}

void buildDistanceMatrix(
    const vector<City> &cities)
{
    int n = cities.size();

    cout << "\n[DEBUG] Building distance matrix...\n";

    auto start =
        chrono::steady_clock::now();

    distMatrix.assign(
        n,
        vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        if (i % 250 == 0)
        {
            cout << "[DEBUG] Row "
                 << i
                 << "/"
                 << n
                 << "\r";

            cout.flush();
        }

        for (int j = i + 1; j < n; j++)
        {
            double dx =
                cities[i].x - cities[j].x;

            double dy =
                cities[i].y - cities[j].y;

            int d =
                (int)round(
                    sqrt(dx * dx + dy * dy));

            distMatrix[i][j] =
                distMatrix[j][i] = d;
        }
    }

    auto end =
        chrono::steady_clock::now();

    cout << "\n[DEBUG] Distance matrix done in "
         << chrono::duration_cast<
                chrono::seconds>(
                end - start)
                .count()
         << " sec\n";
}

//////////////////////////////////////////////////////////////
// NEAREST NEIGHBORS
//////////////////////////////////////////////////////////////

vector<vector<int>> nearest;

void buildNearest(
    const vector<City> &cities)
{
    int n = cities.size();

    cout << "\n[DEBUG] Building nearest lists...\n";

    auto start =
        chrono::steady_clock::now();

    nearest.assign(n, {});

    for (int i = 0; i < n; i++)
    {
        if (i % 250 == 0)
        {
            cout << "[DEBUG] NN "
                 << i
                 << "/"
                 << n
                 << "\r";

            cout.flush();
        }

        vector<pair<int, int>> temp;

        temp.reserve(n - 1);

        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;

            temp.push_back(
                {Dist(i, j), j});
        }

        nth_element(
            temp.begin(),
            temp.begin() + K_NEIGHBORS,
            temp.end());

        nearest[i].reserve(K_NEIGHBORS);

        for (int k = 0;
             k < K_NEIGHBORS;
             k++)
        {
            nearest[i].push_back(
                temp[k].second);
        }
    }

    auto end =
        chrono::steady_clock::now();

    cout << "\n[DEBUG] Nearest lists done in "
         << chrono::duration_cast<
                chrono::seconds>(
                end - start)
                .count()
         << " sec\n";
}

//////////////////////////////////////////////////////////////
// GREEDY INITIAL TOUR
//////////////////////////////////////////////////////////////

vector<int> nearestNeighborTour(
    const vector<City> &cities)
{
    int n = cities.size();

    vector<int> tour;

    vector<char> used(n, false);

    tour.reserve(n);

    int current =
        rng() % n;

    used[current] = true;

    tour.push_back(current);

    for (int step = 1; step < n; step++)
    {
        int best = -1;
        int bestDist = INT_MAX;

        for (int j = 0; j < n; j++)
        {
            if (used[j])
                continue;

            int d = Dist(current, j);

            if (d < bestDist)
            {
                bestDist = d;
                best = j;
            }
        }

        current = best;

        used[current] = true;

        tour.push_back(current);
    }

    return tour;
}

//////////////////////////////////////////////////////////////
// ROUTE COST
//////////////////////////////////////////////////////////////

int routeCost(
    const vector<int> &tour)
{
    int n = tour.size();

    int sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum += Dist(
            tour[i],
            tour[(i + 1) % n]);
    }

    return sum;
}

//////////////////////////////////////////////////////////////
// PERTURBATION
//////////////////////////////////////////////////////////////

void perturbTour(vector<int> &tour)
{
    int n = tour.size();

    for (int k = 0; k < 5; k++)
    {
        int a = rng() % n;
        int b = rng() % n;

        if (a > b)
            swap(a, b);

        reverse(
            tour.begin() + a,
            tour.begin() + b);
    }
}

//////////////////////////////////////////////////////////////
// BEST-IMPROVEMENT 2-OPT
//////////////////////////////////////////////////////////////

pair<int, vector<int>> fast2opt(
    const vector<City> &cities,
    vector<int> tour)
{
    int n = cities.size();

    int cost =
        routeCost(tour);

    vector<int> pos(n);

    for (int i = 0; i < n; i++)
    {
        pos[tour[i]] = i;
    }

    bool improved = true;

    int iteration = 0;

    auto start =
        chrono::steady_clock::now();

    while (improved)
    {
        improved = false;

        iteration++;

        int passImprovements = 0;

        for (int aPos = 0;
             aPos < n;
             aPos++)
        {
            int a = tour[aPos];

            int bPos =
                (aPos + 1) % n;

            int b = tour[bPos];

            int bestDelta = 0;

            int bestBPos = -1;
            int bestCPos = -1;

            ////////////////////////////////////////////////////

            for (int c : nearest[a])
            {
                int cPos = pos[c];

                int dPos =
                    (cPos + 1) % n;

                int d = tour[dPos];

                if (a == c ||
                    a == d ||
                    b == c ||
                    b == d)
                    continue;

                int delta =
                    Dist(a, c) +
                    Dist(b, d) -
                    Dist(a, b) -
                    Dist(c, d);

                if (delta < bestDelta)
                {
                    bestDelta = delta;

                    bestBPos = bPos;
                    bestCPos = cPos;
                }
            }

            ////////////////////////////////////////////////////

            if (bestDelta < 0)
            {
                if (bestBPos < bestCPos)
                {
                    reverse(
                        tour.begin() + bestBPos,
                        tour.begin() + bestCPos + 1);

                    for (int i = bestBPos;
                         i <= bestCPos;
                         i++)
                    {
                        pos[tour[i]] = i;
                    }
                }
                else
                {
                    reverse(
                        tour.begin() + bestCPos + 1,
                        tour.begin() + bestBPos);

                    for (int i = bestCPos + 1;
                         i < bestBPos;
                         i++)
                    {
                        pos[tour[i]] = i;
                    }
                }

                cost += bestDelta;

                improved = true;

                passImprovements++;
            }
        }

        cout << "[DEBUG] Pass "
             << iteration
             << " improvements="
             << passImprovements
             << " cost="
             << cost
             << "\n";

        if (passImprovements == 0)
            break;
    }

    auto end =
        chrono::steady_clock::now();

    cout << "[DEBUG] 2-opt finished in "
         << chrono::duration_cast<
                chrono::seconds>(
                end - start)
                .count()
         << " sec\n";

    return {cost, tour};
}

//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////

int main()
{
    ios::sync_with_stdio(false);

    cin.tie(nullptr);

    //////////////////////////////////////////////////////////

    string fileName =
        "data/ca4663.tsp";

    //////////////////////////////////////////////////////////

    auto globalStart =
        chrono::steady_clock::now();

    //////////////////////////////////////////////////////////

    cout << "[DEBUG] Reading file...\n";

    vector<City> cities =
        readCountry(fileName);

    cout << "[DEBUG] Cities: "
         << cities.size()
         << "\n";

    //////////////////////////////////////////////////////////

    buildDistanceMatrix(cities);

    buildNearest(cities);

    //////////////////////////////////////////////////////////

    int bestCost = INT_MAX;

    vector<int> bestTour;

    //////////////////////////////////////////////////////////

    for (int run = 0;
         run < RUNS;
         run++)
    {
        cout << "\n=================================\n";

        cout << "[DEBUG] RUN "
             << run + 1
             << "/"
             << RUNS
             << "\n";

        //////////////////////////////////////////////////////

        vector<int> startTour;

        if (run == 0)
        {
            startTour =
                nearestNeighborTour(cities);
        }
        else
        {
            startTour =
                bestTour;

            perturbTour(startTour);
        }

        //////////////////////////////////////////////////////

        auto [cost, tour] =
            fast2opt(cities, startTour);

        //////////////////////////////////////////////////////

        if (cost < bestCost)
        {
            bestCost = cost;

            bestTour = move(tour);

            cout << "[DEBUG] NEW BEST = "
                 << bestCost
                 << "\n";
        }

        cout << "[DEBUG] Current global best = "
             << bestCost
             << "\n";
    }

    //////////////////////////////////////////////////////////

    auto globalEnd =
        chrono::steady_clock::now();

    //////////////////////////////////////////////////////////

    cout << "\n=================================\n";

    cout << "FINAL BEST COST: "
         << bestCost
         << "\n";

    cout << "TOTAL TIME: "
         << chrono::duration_cast<
                chrono::seconds>(
                globalEnd - globalStart)
                .count()
         << " sec\n";

    //////////////////////////////////////////////////////////
    // SAVE ROUTE
    //////////////////////////////////////////////////////////

    ofstream out("best_route.txt");

    out << "COST "
        << bestCost
        << "\n";

    for (int v : bestTour)
    {
        out << cities[v].id
            << " ";
    }

    out << "\n";

    cout << "[DEBUG] Route saved to best_route.txt\n";

    return 0;
}