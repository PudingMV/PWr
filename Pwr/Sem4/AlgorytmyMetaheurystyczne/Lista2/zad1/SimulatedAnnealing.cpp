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

inline int dist(const City &a, const City &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (int)round(sqrt(dx * dx + dy * dy));
}

int routeCost(const vector<City> &c, const vector<int> &perm) {
    int n = perm.size();

    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += dist(c[perm[i]], c[perm[(i + 1) % n]]);
    }

    return sum;
}

vector<vector<int>> buildCandidates(
    const vector<City> &cities,
    int K = 20
) {
    int n = cities.size();

    vector<vector<int>> cand(n);

    for (int i = 0; i < n; i++) {

        vector<pair<int,int>> d;

        for (int j = 0; j < n; j++) {

            if (i == j)
                continue;

            d.push_back({
                dist(cities[i], cities[j]),
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
    const vector<City> &c,
    const vector<int> &r,
    int i,
    int j
) {
    int n = r.size();

    int a = r[i];
    int b = r[(i + 1) % n];

    int c1 = r[j];
    int d = r[(j + 1) % n];

    return
        (dist(c[a], c[c1]) + dist(c[b], c[d]))
        -
        (dist(c[a], c[b]) + dist(c[c1], c[d]));
}

inline void apply2opt(vector<int> &r, int i, int j) {
    reverse(r.begin() + i + 1, r.begin() + j + 1);
}

vector<int> randomRoute(int n) {

    vector<int> r(n);

    iota(r.begin(), r.end(), 0);

    shuffle(r.begin(), r.end(), rng);

    return r;
}

struct SAParams {
    double T0;
    double Tmin;
    double cooling;

    int trials;
    int maxEpochs;
};

SAParams getParams(int n) {

    SAParams p;

    p.T0 = 5000.0;
    p.Tmin = 1e-3;

    if (n < 100) {

        p.cooling = 0.995;
        p.trials = 5 * n;
        p.maxEpochs = 2000;

    }
    else if (n < 1000) {

        p.cooling = 0.997;
        p.trials = 8 * n;
        p.maxEpochs = 4000;

    }
    else if (n < 5000) {

        p.cooling = 0.9985;
        p.trials = 12 * n;
        p.maxEpochs = 6000;

    }
    else {

        p.cooling = 0.999;
        p.trials = 15 * n;
        p.maxEpochs = 8000;
    }

    p.T0 *= sqrt((double)n / 100.0);

    return p;
}

pair<int, vector<int>> simulatedAnnealing(
    const vector<City> &cities,
    double T0,
    double Tmin,
    double cooling,
    int trialsPerEpoch,
    int maxEpochs,
    const vector<vector<int>> &cand,
    bool verbose = false
) {
    int n = cities.size();

    vector<int> cur = randomRoute(n);

    int curCost = routeCost(cities, cur);

    vector<int> best = cur;

    int bestCost = curCost;

    vector<int> pos(n);

    for (int i = 0; i < n; i++) {
        pos[cur[i]] = i;
    }

    double T = T0;

    int epoch = 0;

    while (T > Tmin && epoch < maxEpochs) {

        epoch++;

        for (int t = 0; t < trialsPerEpoch; t++) {

            int i = rndInt(0, n - 1);

            int cityA = cur[i];

            int cityB;

            if (!cand[cityA].empty() && uni01(rng) < 0.8) {

                cityB =
                    cand[cityA][
                        rndInt(0, (int)cand[cityA].size() - 1)
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

            int d = delta2opt(cities, cur, i, j);

            if (d < 0 || uni01(rng) < exp(-(double)d / T)) {

                apply2opt(cur, i, j);

                for (int k = i + 1; k <= j; k++) {
                    pos[cur[k]] = k;
                }

                curCost += d;

                if (curCost < bestCost) {

                    bestCost = curCost;

                    best = cur;

                    if (verbose) {
                        cout << "\nNEW BEST: "
                             << bestCost
                             << " at epoch "
                             << epoch
                             << "\n";
                    }
                }
            }
        }

        if (verbose && epoch % 50 == 0) {

            cout
                << "Epoch: " << epoch
                << " | Temp: " << T
                << " | Current: " << curCost
                << " | Best: " << bestCost
                << "\r";

            cout.flush();
        }
        T *= cooling;
    }

    if (verbose)
        cout << "\n";

    return {bestCost, best};
}

void solve(const vector<string> &files) {

    for (auto &file : files) {

        ifstream in("data/" + file);

        if (!in)
            continue;

        vector<City> cities;

        string line;

        bool ok = false;

        while (getline(in, line)) {

            if (line.find("NODE_COORD_SECTION") != string::npos) {
                ok = true;
                continue;
            }

            if (!ok)
                continue;

            if (line.find("EOF") != string::npos)
                break;

            stringstream ss(line);

            City c;

            if (ss >> c.id >> c.x >> c.y) {
                cities.push_back(c);
            }
        }

        int n = cities.size();

        cout << "\nFILE: "
             << file
             << " | N="
             << n
             << "\n";

        auto cand = buildCandidates(cities, 20);

        SAParams p = getParams(n);

        cout << "T0 = " << p.T0 << "\n";
        cout << "Cooling = " << p.cooling << "\n";
        cout << "Trials = " << p.trials << "\n";
        cout << "Max epochs = " << p.maxEpochs << "\n";

        int RUNS = (n < 1000 ? n : 100);

        long long bestAll = LLONG_MAX;

        vector<int> bestRoute;

        auto start = chrono::high_resolution_clock::now();

        for (int r = 0; r < RUNS; r++) {

            bool verbose = (n < 1000);

            auto [cost, route] =
                simulatedAnnealing(
                    cities,
                    p.T0,
                    p.Tmin,
                    p.cooling,
                    p.trials,
                    p.maxEpochs,
                    cand,
                    verbose
                );

            if (cost < bestAll) {

                bestAll = cost;

                bestRoute = route;
            }

            cout
                << "Run "
                << r + 1
                << "/"
                << RUNS
                << " cost="
                << cost
                << "\n";
        }

        auto end = chrono::high_resolution_clock::now();

        double sec =
            chrono::duration<double>(end - start).count();

        cout
            << "\nBEST: "
            << bestAll
            << "\nTIME: "
            << sec
            << " s\n";

        ofstream out("SA_" + file);

        out << "Best cost: " << bestAll << "\n";

        out << "Time: " << sec << "\n";

        out << "Route:\n";

        for (int v : bestRoute)
            out << cities[v].id << " ";

        out << "\n";
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

    solve(files);
}