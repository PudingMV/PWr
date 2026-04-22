#include <bits/stdc++.h>
using namespace std;

const int N = 10;

string fileNames[N] = {"wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp", "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"};

struct City {
    int id;
    double x;
    double y;
};

vector <City> readCountry(string fileName)
{
    ifstream file(fileName);

    string line;
    vector<City> cities;

    bool readCoords = false;

    while (getline(file, line))
    {
        
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            readCoords = true;
            continue;
        }
        
        if (line.find("EOF") != string::npos) break;
        
        if (readCoords) {
            stringstream ss(line);
    
            int id;
            float x, y;
            char comma1, comma2;
    
            ss >> id >> comma1 >> x >> comma2 >> y;
            City c;
            c.id = id;
            c.x = x;
            c.y = y;
            cities.push_back(c);
        }
    }

    return cities;
}

int Distance(City a, City b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (int) round(sqrt(dx*dx + dy*dy));
}

vector <vector <int>> calculateDistances(vector <City> cities)
{
    int n = cities.size();
    vector <vector <int>> distances(n, vector<int>(0)); 
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            distances[i].push_back(Distance(cities[i], cities[j]));
        }
    }

    return distances;

}

void randomPermutate(vector <int> &arr, int size)
{
    for(int i=size-1;i>=1;i--)
    {
        int index = rand() % i + 1;
        int temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }
}

int calculateRoute(vector<vector<int>> distances, int permutation[], int n)
{
    int length = 0;
    for(int i = 1;i < n; i++)
    {
        length+=distances[permutation[i-1]-1][permutation[i]-1];
    }

    length += distances[permutation[n-1]-1][permutation[0]-1];

    return length;
}

void assignArray(int permutation1[], int permutation2[], int n)
{
    for(int i = 0; i < n; i++)
    {
        permutation2[i] = permutation1[i];
    }
}

void getNeighbourhooded(int permutation[], int n)
{
    int first = (rand() % (n-1)) + 1;
    int second = rand() % first;

    int temp = permutation[second];

    permutation[second] = permutation[first];
    permutation[first] = temp;
}

pair<int, vector<int>> LocalSearch(vector<City> cities, int &steps)
{
    vector<vector<int>> distances = calculateDistances(cities);
    int n = cities.size();

    vector<int> current(n);
    vector<int> bestSoFar(n);

    for(int i = 0; i < n; i++)
        current[i] = i+1;

    randomPermutate(current, n);

    bestSoFar = current;
    int bestCostSoFar = calculateRoute(distances, current.data(), n);

    steps = 0;

    while(true)
    {
        vector<int> bestNeighbour = current;
        int bestCost = calculateRoute(distances, current.data(), n);

        bool improved = false;

        for(int i = 0; i < n - 1; i++)
        {
            for(int j = i + 1; j < n; j++)
            {
                vector<int> candidate = current;

                reverse(candidate.begin() + i, candidate.begin() + j + 1);

                int c = calculateRoute(distances, candidate.data(), n);

                if(c < bestCost)
                {
                    bestCost = c;
                    bestNeighbour = candidate;
                    improved = true;
                }
            }
        }

        current = bestNeighbour;

        int currentCost = calculateRoute(distances, current.data(), n);
        if(currentCost < bestCostSoFar)
        {
            bestCostSoFar = currentCost;
            bestSoFar = current;
        }

        if(!improved)
            break;

        steps++;
    }

    return {bestCostSoFar, bestSoFar};
}

void Solve()
{
    for(int f = 0; f < N; f++)
    {
        vector<City> cities = readCountry("data/" + fileNames[f]);
        int n = cities.size();

        double sum = 0;
        double stepsSum = 0;
        int best = INT_MAX;
        vector<int> bestPath;

        for(int k = 0; k < n; k++)
        {
            int steps = 0;

            auto result = LocalSearch(cities, steps);

            int cost = result.first;
            vector<int> path = result.second;

            sum += cost;
            stepsSum += steps;

            if(cost < best)
            {
                best = cost;
                bestPath = path;
            }
        }

        // 📊 statystyki
        cout << fileNames[f] << "\n";
        cout << "avg cost: " << sum / n << "\n";
        cout << "avg steps: " << stepsSum / n << "\n";
        cout << "best: " << best << "\n\n";

        // 💾 zapis najlepszej ścieżki
        ofstream pathFile("results/task1_path_" + fileNames[f]);

        for(int v : bestPath)
            pathFile << v << " ";

        pathFile << "\n";
    }
}


int main()
{
    Solve();
}