#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

thread_local mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

inline int rndInt(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}
inline double rnd01() {
    return uniform_real_distribution<double>(0.0, 1.0)(rng);
}

struct City {
    int id;
    double x, y;
};

vector<vector<int>> distMat;

inline int D(int a, int b) {
    return distMat[a][b];
}

void buildDistanceMatrix(const vector<City>& c) {
    int n = c.size();
    distMat.assign(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {

            double dx = c[i].x - c[j].x;
            double dy = c[i].y - c[j].y;

            int d = (int)(sqrt(dx * dx + dy * dy) + 0.5);

            distMat[i][j] = d;
            distMat[j][i] = d;
        }
    }
}

inline int routeCost(const vector<int>& r) {
    int s = 0;

    int n = r.size();

    for (int i = 0; i < n; i++) {
        s += D(r[i], r[(i + 1) % n]);
    }

    return s;
}

struct Individual {
    vector<int> route;
    vector<int> pos;
    int cost;
};

inline void buildPos(Individual &ind) {
    int n = ind.route.size();
    ind.pos.assign(n, 0);
    for (int i = 0; i < n; i++)
        ind.pos[ind.route[i]] = i;
}

vector<int> nearestNeighbor(int n) {
    vector<int> route;
    route.reserve(n);

    vector<char> used(n, 0);
    int cur = rndInt(0, n - 1);

    route.push_back(cur);
    used[cur] = 1;

    for (int i = 1; i < n; i++) {
        int best = -1;
        double bestd = 1e18;

        for (int j = 0; j < n; j++) {
            if (!used[j]) {
                double d = D(cur, j);
                if (d < bestd) {
                    bestd = d;
                    best = j;
                }
            }
        }

        cur = best;
        used[cur] = 1;
        route.push_back(cur);
    }

    return route;
}

vector<int> randomRoute(int n) {
    vector<int> r(n);
    iota(r.begin(), r.end(), 0);
    shuffle(r.begin(), r.end(), rng);
    return r;
}

vector<int> initRoute(int n) {
    if (rnd01() < 0.2)
        return nearestNeighbor(n);
    return randomRoute(n);
}

vector<vector<int>> buildCandidates(const vector<City>& cities, int K = 15) {
    int n = cities.size();
    vector<vector<int>> cand(n);

    vector<pair<double,int>> tmp;

    for (int i = 0; i < n; i++) {
        tmp.clear();

        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            tmp.push_back({D(i, j), j});
        }

        nth_element(tmp.begin(), tmp.begin() + K, tmp.end(),
                    [](auto &a, auto &b){ return a.first < b.first; });

        sort(tmp.begin(), tmp.begin() + K,
             [](auto &a, auto &b){ return a.first < b.first; });

        cand[i].resize(K);
        for (int k = 0; k < K; k++)
            cand[i][k] = tmp[k].second;
    }

    return cand;
}

vector<Individual> createPopulation(int n, int popSize) {
    vector<Individual> pop(popSize);

    #pragma omp parallel for
    for (int i = 0; i < popSize; i++) {
        pop[i].route = initRoute(n);
        pop[i].cost = routeCost(pop[i].route);

        pop[i].pos.assign(n, 0);
        for (int j = 0; j < n; j++)
            pop[i].pos[pop[i].route[j]] = j;
    }

    return pop;
}

const Individual& tournament(const vector<Individual>& pop, int k) {
    int best = rndInt(0, pop.size() - 1);

    for (int i = 1; i < k; i++) {
        int idx = rndInt(0, pop.size() - 1);
        if (pop[idx].cost < pop[best].cost)
            best = idx;
    }
    return pop[best];
}

vector<int> crossoverOX(const vector<int>& p1, const vector<int>& p2) {
    int n = p1.size();
    vector<int> child(n, -1);
    vector<char> used(n, 0);

    int a = rndInt(0, n - 2);
    int b = rndInt(a + 1, n - 1);

    for (int i = a; i <= b; i++) {
        child[i] = p1[i];
        used[p1[i]] = 1;
    }

    int idx = (b + 1 == n ? 0 : b + 1);

    for (int i = 0; i < n; i++) {
        int gene = p2[(b + 1 + i) % n];
        if (!used[gene]) {
            child[idx] = gene;
            used[gene] = 1;
            idx = (idx + 1 == n ? 0 : idx + 1);
        }
    }

    return child;
}

void mutate(vector<int>& r) {
    int n = r.size();
    int i = rndInt(0, n - 2);
    int j = rndInt(i + 1, n - 1);
    reverse(r.begin() + i, r.begin() + j + 1);
}

void doubleBridge(vector<int>& r)
{
    int n = r.size();

    if (n < 8) return;

    int a = rndInt(1, n / 4);
    int b = rndInt(a + 1, n / 2);
    int c = rndInt(b + 1, 3 * n / 4);

    vector<int> t;
    t.reserve(n);

    t.insert(t.end(), r.begin(),     r.begin() + a);
    t.insert(t.end(), r.begin() + c, r.end());
    t.insert(t.end(), r.begin() + b, r.begin() + c);
    t.insert(t.end(), r.begin() + a, r.begin() + b);

    r.swap(t);
}

void localImprove(vector<int>& route,
                  vector<int>& pos,
                  const vector<vector<int>>& cand)
{
    int n = route.size();

    for (int iter = 0; iter < 10; iter++) {
        bool improved = false;

        for (int i = 0; i < n - 1; i++) {
            int a = route[i];
            int b = route[(i + 1 == n ? 0 : i + 1)];

            for (int t = 0; t < 15; t++) {
                int c = cand[a][t];
                int j = pos[c];

                if (j <= i + 1) continue;

                int c1 = route[j];
                int d  = route[(j + 1 == n ? 0 : j + 1)];

                double gain =
                    (D(a, c1) + D(b, d)) -
                    (D(a, b) + D(c1, d));

                if (gain < 0) {
                    reverse(route.begin() + i + 1, route.begin() + j + 1);

                    for (int k = i + 1; k <= j; k++)
                        pos[route[k]] = k;

                    improved = true;
                }
            }
        }

        if (!improved) break;
    }
}

struct Island {
    vector<Individual> pop;
    Individual best;
    double mutationRate = 0.02;
    int stagnant = 0;
};

vector<Island> initIslands(int n, int islands, int popSize) {
    vector<Island> res(islands);

    #pragma omp parallel for
    for (int i = 0; i < islands; i++) {
        res[i].pop = createPopulation(n, popSize);

        int best = 0;
        for (int j = 1; j < popSize; j++)
            if (res[i].pop[j].cost < res[i].pop[best].cost)
                best = j;

        res[i].best = res[i].pop[best];
    }

    return res;
}

void GA_step(Island &island,
             const vector<vector<int>>& cand,
             int popSize)
{
    auto &pop = island.pop;

    int best = 0;
    for (int i = 1; i < pop.size(); i++)
        if (pop[i].cost < pop[best].cost)
            best = i;

    if (pop[best].cost < island.best.cost) {
        island.best = pop[best];
        island.stagnant = 0;
    } else {
        island.stagnant++;
    }

    double pm = island.mutationRate;
    if (island.stagnant > 100) pm = 0.10;
    if (island.stagnant > 300) pm = 0.20;
    if (island.stagnant > 500) pm = 0.30;

    vector<Individual> next(popSize);

    vector<int> idx(popSize);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(),
         [&](int a, int b){ return pop[a].cost < pop[b].cost; });

    for (int i = 0; i < 10; i++)
        next[i] = pop[idx[i]];

    #pragma omp parallel for
    for (int i = 10; i < popSize; i++) {

        const auto& p1 = tournament(pop, 4);
        const auto& p2 = tournament(pop, 4);

        vector<int> child = crossoverOX(p1.route, p2.route);

    if (rnd01() < pm)
    {
        if (rnd01() < 0.7)
            mutate(child);
        else
            doubleBridge(child);
    }

        Individual ind;
        ind.route = move(child);

        buildPos(ind);

        if (rnd01() < 0.3)
            localImprove(ind.route, ind.pos, cand);

        ind.cost = routeCost(ind.route);

        next[i] = move(ind);
    }

    pop = move(next);
}

void migrate(vector<Island>& islands) {
    int k = islands.size();

    for (int i = 0; i < k; i++) {
        auto best = islands[i].best;

        auto &target = islands[(i + 1) % k].pop;

        int worst = 0;
        for (int j = 1; j < target.size(); j++)
            if (target[j].cost > target[worst].cost)
                worst = j;

        target[worst] = best;
    }
}

void solve(const vector<string>& files) {

    for (auto &file : files) {

        ifstream in("data/" + file);
        if (!in) continue;

        vector<City> cities;
        string line;
        bool ok = false;

        while (getline(in, line)) {
            if (line.find("NODE_COORD_SECTION") != string::npos) {
                ok = true;
                continue;
            }
            if (!ok) continue;
            if (line.find("EOF") != string::npos) break;

            stringstream ss(line);
            City c;
            ss >> c.id >> c.x >> c.y;
            cities.push_back(c);
        }

        int n = cities.size();

        buildDistanceMatrix(cities);

        auto cand = buildCandidates(cities, min(40, n-1));

        int islandsCount = 5;
        int popSize = 1000;
        int generations = 3000;

        auto islands = initIslands(n, islandsCount, popSize);

        Individual globalBest;
        globalBest.cost = INT_MAX;

        auto start = chrono::high_resolution_clock::now();

        for (int g = 0; g < generations; g++) {

            #pragma omp parallel for
            for (int i = 0; i < islands.size(); i++) {
                GA_step(islands[i], cand, popSize);
            }

            for (auto &island : islands)
                if (island.best.cost < globalBest.cost)
                    globalBest = island.best;

            if (g % 100 == 0)
                migrate(islands);

            if (g % 50 == 0)
                cout << "Gen " << g
                     << " best=" << globalBest.cost << "\n" << flush;
        }

        auto end = chrono::high_resolution_clock::now();
        double sec = chrono::duration<double>(end - start).count();

        cout << "\nBEST: " << globalBest.cost
            << "\nTIME: "
            << sec
            << " s\n";

        ofstream out("IslandGA_" + file);

        out << "Best cost: " << globalBest.cost << "\n";
        out << "Time: " << sec << "\n";
        out << "Route:\n";

        for (int v : globalBest.route)
            out << cities[v].id << " ";

        out << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> files = {
        "wi29.tsp","dj38.tsp","qa194.tsp","uy734.tsp","zi929.tsp",
        "mu1979.tsp","ca4663.tsp","tz6117.tsp","eg7146.tsp","ei8246.tsp"
    };

    solve(files);
}