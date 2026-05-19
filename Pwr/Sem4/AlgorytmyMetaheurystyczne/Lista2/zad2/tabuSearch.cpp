#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

uniform_real_distribution<double> uni01(0.0, 1.0);

inline int rndInt(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

struct City {
    int id;
    double x, y;
};

inline int Distance(const vector<City> &cities, int i, int j) {

    double dx = cities[i].x - cities[j].x;
    double dy = cities[i].y - cities[j].y;

    return (int)round(sqrt(dx * dx + dy * dy));
}

int calculateRoute(
    const vector<City> &cities,
    const vector<int> &perm
) {
    int n = perm.size();

    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += Distance(
            cities,
            perm[i],
            perm[(i + 1) % n]
        );
    }

    return sum;
}

vector<int> randomRoute(int n) {

    vector<int> r(n);

    iota(r.begin(), r.end(), 0);

    shuffle(r.begin(), r.end(), rng);

    return r;
}

vector<vector<int>> buildCandidates(
    const vector<City> &cities,
    int K
) {
    int n = cities.size();

    vector<vector<int>> cand(n);

    for (int i = 0; i < n; i++) {

        vector<pair<int,int>> d;

        d.reserve(n);

        for (int j = 0; j < n; j++) {

            if (i == j)
                continue;

            d.push_back({
                Distance(cities, i, j),
                j
            });
        }

        sort(d.begin(), d.end());

        for (int k = 0; k < min(K, (int)d.size()); k++) {
            cand[i].push_back(d[k].second);
        }
    }

    return cand;
}

inline int delta2opt(
    const vector<City> &cities,
    const vector<int> &route,
    int i,
    int j
) {
    int n = route.size();

    int a = route[i];
    int b = route[(i + 1) % n];

    int c = route[j];
    int d = route[(j + 1) % n];

    return
        (Distance(cities, a, c) +
         Distance(cities, b, d))
        -
        (Distance(cities, a, b) +
         Distance(cities, c, d));
}

inline void applyMove(
    vector<int> &route,
    int i,
    int j
) {
    reverse(
        route.begin() + i + 1,
        route.begin() + j + 1
    );
}

struct TabuParams {

    int tabuLength;
    int maxIterations;

    int neighbourhoodSize;

    int maxNoImprovement;
};

TabuParams getParams(int n) {

    TabuParams p;

    if (n < 100) {

        p.tabuLength = 20;

        p.maxIterations = 2000;

        p.neighbourhoodSize = 200;

        p.maxNoImprovement = 300;
    }

    else if (n < 1000) {

        p.tabuLength = 100;

        p.maxIterations = 10000;

        p.neighbourhoodSize = 10000;

        p.maxNoImprovement = 1000;
    }

    else if (n < 5000) {

        p.tabuLength = 200;

        p.maxIterations = 20000;

        p.neighbourhoodSize = 30000;

        p.maxNoImprovement = 5000;
    }

    else {

        p.tabuLength = 200;

        p.maxIterations = 30000;

        p.neighbourhoodSize = 50000;

        p.maxNoImprovement = 10000;
    }

    return p;
}

pair<int, vector<int>> TabuSearch(
    const vector<City> &cities,
    int tabuLength,
    int maxIterations,
    int neighbourhoodSize,
    int maxNoImprovement,
    const vector<vector<int>> &cand,
    int &steps,
    bool verbose = false
) {
    int n = cities.size();

    vector<int> current = randomRoute(n);

    int currentCost =
        calculateRoute(cities, current);

    vector<int> best = current;

    int bestCost = currentCost;

    vector<int> pos(n);

    for (int i = 0; i < n; i++) {
        pos[current[i]] = i;
    }

    unordered_set<long long> tabuSet;

    deque<long long> tabuQueue;

    auto encode = [&](int a, int b) -> long long {

        if (a > b)
            swap(a, b);

        return 1LL * a * n + b;
    };

    int noImprovement = 0;

    steps = 0;

    for (int iter = 1;
         iter <= maxIterations;
         iter++) {

        if (verbose && iter % 100 == 0) {

            cout
                << "Iter: " << iter
                << " | Current: " << currentCost
                << " | Best: " << bestCost
                << "\r";

            cout.flush();
        }

        int bestDelta = INT_MAX;

        int bestI = -1;
        int bestJ = -1;

        for (int s = 0; s < neighbourhoodSize; s++) {

            int i = rndInt(0, n - 1);

            int cityA = current[i];

            int cityB;

            if (!cand[cityA].empty()
                && uni01(rng) < 0.8) {

                cityB =
                    cand[cityA][
                        rndInt(
                            0,
                            (int)cand[cityA].size() - 1
                        )
                    ];

            } else {

                cityB = rndInt(0, n - 1);
            }

            int j = pos[cityB];

            if (i == j)
                continue;

            if (i > j)
                swap(i, j);

            if (j == i + 1)
                continue;

            if (i == 0 && j == n - 1)
                continue;

            int delta =
                delta2opt(
                    cities,
                    current,
                    i,
                    j
                );

            long long code =
                encode(current[i], current[j]);

            bool isTabu =
                tabuSet.count(code);

            int newCost =
                currentCost + delta;

            if (isTabu &&
                newCost >= bestCost)
                continue;

            if (delta < bestDelta) {

                bestDelta = delta;

                bestI = i;
                bestJ = j;
            }
        }

        if (bestI == -1)
            break;

        applyMove(current, bestI, bestJ);

        for (int k = bestI + 1;
             k <= bestJ;
             k++) {

            pos[current[k]] = k;
        }

        currentCost += bestDelta;

        long long code =
            encode(
                current[bestI],
                current[bestJ]
            );

        tabuSet.insert(code);

        tabuQueue.push_back(code);

        if ((int)tabuQueue.size()
            > tabuLength) {

            tabuSet.erase(
                tabuQueue.front()
            );

            tabuQueue.pop_front();
        }

        if (currentCost < bestCost) {

            bestCost = currentCost;

            best = current;

            noImprovement = 0;

            if (verbose) {

                cout
                    << "\nNEW BEST: "
                    << bestCost
                    << "\n";
            }

        } else {

            noImprovement++;
        }

        if (noImprovement
            >= maxNoImprovement) {

            if (verbose)
                cout
                    << "\nSTOP: no improvement\n";

            break;
        }

        steps++;
    }

    if (verbose)
        cout << "\n";

    return {bestCost, best};
}

void solveExample(
    const vector<string> &files
) {
    for (auto &file : files) {

        ifstream in("data/" + file);

        if (!in)
            continue;

        vector<City> cities;

        string line;

        bool ok = false;

        while (getline(in, line)) {

            if (line.find(
                "NODE_COORD_SECTION"
                ) != string::npos) {

                ok = true;

                continue;
            }

            if (!ok)
                continue;

            if (line.find("EOF")
                != string::npos)
                break;

            stringstream ss(line);

            City c;

            if (ss >> c.id
                   >> c.x
                   >> c.y) {

                cities.push_back(c);
            }
        }

        int n = cities.size();

        cout
            << "\nFILE: "
            << file
            << " | N="
            << n
            << "\n";

        TabuParams p = getParams(n);

        cout
            << "Tabu length: "
            << p.tabuLength
            << "\n";

        cout
            << "Max iterations: "
            << p.maxIterations
            << "\n";

        cout
            << "Neighborhood: "
            << p.neighbourhoodSize
            << "\n";

        int K = (n < 1000 ? 20 : 40);

        auto cand =
            buildCandidates(cities, K);

        int RUNS =
            (n < 1000 ? n : 100);

        long long bestAll = LLONG_MAX;

        vector<int> bestRoute;

        double sumCost = 0;

        auto start =
            chrono::high_resolution_clock::now();

        for (int r = 0;
             r < RUNS;
             r++) {

            int steps = 0;

            auto [cost, route] =
                TabuSearch(
                    cities,
                    p.tabuLength,
                    p.maxIterations,
                    p.neighbourhoodSize,
                    p.maxNoImprovement,
                    cand,
                    steps,
                    n < 1000
                );

            cout
                << "Run "
                << r + 1
                << "/"
                << RUNS
                << " cost="
                << cost
                << "\n";

            sumCost += cost;

            if (cost < bestAll) {

                bestAll = cost;

                bestRoute = route;
            }
        }

        auto end =
            chrono::high_resolution_clock::now();

        double sec =
            chrono::duration<double>(
                end - start
            ).count();

        string outName =
            "tabu_stats_" + file;

        ofstream out(outName);

        out << "FILE: "
            << file
            << "\n";

        out << "Cities: "
            << n
            << "\n";

        out << "Runs: "
            << RUNS
            << "\n\n";

        out << "Tabu length: "
            << p.tabuLength
            << "\n";

        out << "Max iterations: "
            << p.maxIterations
            << "\n";

        out << "Neighborhood size: "
            << p.neighbourhoodSize
            << "\n";

        out << "Stop (no improvement): "
            << p.maxNoImprovement
            << "\n\n";

        out << "Best cost: "
            << bestAll
            << "\n";

        out << "Average cost: "
            << sumCost / RUNS
            << "\n";

        out << "Execution time: "
            << sec
            << " seconds\n\n";

        out << "Best route:\n";

        for (int v : bestRoute)
            out << cities[v].id << " ";

        out << "\n";

        cout
            << "\nBEST: "
            << bestAll
            << "\nAVG: "
            << sumCost / RUNS
            << "\nTIME: "
            << sec
            << " s\n";

        cout
            << "Saved to: "
            << outName
            << "\n";
    }
}

int main() {

    ios::sync_with_stdio(false);

    cin.tie(nullptr);

    vector<string> files = {

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

    solveExample(files);
}