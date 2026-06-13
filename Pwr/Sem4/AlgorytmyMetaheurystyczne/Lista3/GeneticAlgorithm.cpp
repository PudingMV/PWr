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

        int RUNS = (n < 1000 ? n : 100);

        long long bestAll = LLONG_MAX;

        vector<int> bestRoute;

        auto start = chrono::high_resolution_clock::now();

        for (int r = 0; r < RUNS; r++) {

            bool verbose = (n < 1000);


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