#include <bits/stdc++.h>
using namespace std;

using Edge = tuple<int, int, double>;

vector<Edge> generateEdges(int n)
{
    vector<Edge> edges;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    edges.reserve((long long)n * (n - 1) / 2);

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double w = dis(gen);
            edges.emplace_back(i, j, w);
        }
    }

    return edges;
}

struct DSU
{
    vector<int> parent;
    vector<int> rank_;

    DSU(int n)
    {
        parent.resize(n);
        rank_.assign(n, 0);

        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x)
    {
        if (parent[x] == x)
            return x;

        return parent[x] = find(parent[x]);
    }

    bool unite(int a, int b)
    {
        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (rank_[a] < rank_[b])
            swap(a, b);

        parent[b] = a;

        if (rank_[a] == rank_[b])
            rank_[a]++;

        return true;
    }
};

vector<vector<int>> kruskalMST(int n, vector<Edge> edges)
{
    sort(edges.begin(), edges.end(),
         [](const Edge &a, const Edge &b)
         {
             return get<2>(a) < get<2>(b);
         });

    DSU dsu(n);

    vector<vector<int>> tree(n);

    int usedEdges = 0;

    for (auto &[u, v, w] : edges)
    {
        if (dsu.unite(u, v))
        {
            tree[u].push_back(v);
            tree[v].push_back(u);

            usedEdges++;

            if (usedEdges == n - 1)
                break;
        }
    }

    return tree;
}

vector<vector<int>> tree;
vector<int> dp;

void dfs(int v, int parent)
{
    vector<pair<int, int>> children;

    for (int u : tree[v])
    {
        if (u == parent)
            continue;

        dfs(u, v);

        children.push_back({dp[u], u});
    }

    sort(children.begin(),
         children.end(),
         greater<pair<int, int>>());

    dp[v] = 0;

    for (int i = 0; i < (int)children.size(); i++)
    {
        dp[v] = max(
            dp[v],
            i + 1 + children[i].first);
    }
}

int broadcastRounds(
    const vector<vector<int>> &mst,
    int root)
{
    tree = mst;

    int n = tree.size();

    dp.assign(n, 0);

    dfs(root, -1);

    return dp[root];
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    random_device rd;
    mt19937 gen(rd());

    ofstream file("broadcast_results.csv");

    file << "n,min,avg,max\n";

    int nMin = 1000;
    int nMax = 10000;
    int step = 100;

    int repetitions = 30;

    for (int n = nMin; n <= nMax; n += step)
    {
        cout << "n = " << n << endl;

        long long sumRounds = 0;

        int minRounds = INT_MAX;
        int maxRounds = 0;

        for (int rep = 0; rep < repetitions; rep++)
        {
            auto edges = generateEdges(n);

            auto mst = kruskalMST(n, edges);

            uniform_int_distribution<> rootDist(0, n - 1);

            int root = rootDist(gen);

            int rounds =
                broadcastRounds(mst, root);

            sumRounds += rounds;

            minRounds = min(minRounds, rounds);
            maxRounds = max(maxRounds, rounds);

            cout
                << "\r"
                << "  repetition "
                << rep + 1
                << "/"
                << repetitions
                << flush;
        }

        cout << endl;

        double avgRounds =
            (double)sumRounds / repetitions;

        file
            << n << ","
            << minRounds << ","
            << avgRounds << ","
            << maxRounds << "\n";
    }

    file.close();

    cout << "\nWyniki zapisane do broadcast_results.csv\n";

    return 0;
}