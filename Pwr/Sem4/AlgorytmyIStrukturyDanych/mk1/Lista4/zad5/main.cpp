#include <bits/stdc++.h>
using namespace std;

using Edge = tuple<int,int,double>;

vector<Edge> generateEdges(int n, vector<vector<double>>& adj) {
    vector<Edge> edges;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    adj.assign(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double w = dis(gen);
            adj[i][j] = adj[j][i] = w;
            edges.emplace_back(i, j, w);
        }
    }

    return edges;
}

struct DSU {
    vector<int> parent, rnk;

    DSU(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        return true;
    }
};

double kruskal(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end(),
         [](auto &a, auto &b) {
             return get<2>(a) < get<2>(b);
         });

    DSU dsu(n);
    double cost = 0;

    for (auto &[u,v,w] : edges) {
        if (dsu.unite(u, v))
            cost += w;
    }

    return cost;
}

double prim(int n, vector<vector<double>>& adj) {
    vector<bool> used(n, false);
    vector<double> minEdge(n, 1e9);

    minEdge[0] = 0;
    double cost = 0;

    for (int i = 0; i < n; i++) {
        int v = -1;

        for (int j = 0; j < n; j++) {
            if (!used[j] && (v == -1 || minEdge[j] < minEdge[v]))
                v = j;
        }

        used[v] = true;
        cost += minEdge[v];

        for (int to = 0; to < n; to++) {
            if (adj[v][to] < minEdge[to])
                minEdge[to] = adj[v][to];
        }
    }

    return cost;
}
int main() {
    ios::sync_with_stdio(false);

    int nMin = 1000;
    int nMax = 10000;
    int step = 100;
    int rep = 30;

    ofstream file("results.csv");
    file << "n,prim,kruskal\n";

    random_device rd;
    mt19937 gen(rd());

    for (int n = nMin; n <= nMax; n += step) {

        double primTime = 0;
        double kruskalTime = 0;

        for (int r = 0; r < rep; r++) {
            cout << "n=" << n << ", rep=" << r + 1 << "/" << rep << "\r" << flush;

            vector<vector<double>> adj;
            auto edges = generateEdges(n, adj);

            auto start = chrono::high_resolution_clock::now();
            prim(n, adj);
            auto end = chrono::high_resolution_clock::now();
            primTime += chrono::duration<double>(end - start).count();

            start = chrono::high_resolution_clock::now();
            kruskal(n, edges);
            end = chrono::high_resolution_clock::now();
            kruskalTime += chrono::duration<double>(end - start).count();
        }

        primTime /= rep;
        kruskalTime /= rep;

        file << n << "," << primTime << "," << kruskalTime << "\n";

        cout << "n=" << n << " done\n";
    }

    file.close();
    cout << "Saved to results.csv\n";

    return 0;
}