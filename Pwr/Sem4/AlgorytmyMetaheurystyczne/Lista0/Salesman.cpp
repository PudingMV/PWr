#include <bits/stdc++.h>

using namespace std;

const int N = 5;

string fileNames[N] = {"wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp"};

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

void randomPermutate(int arr[], int size)
{
    for(int i=size-1;i>=1;i--)
    {
        int index = rand() % i + 1;
        int temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }
}

void evaluateMin(int n, vector<vector<int>> distances, int batchSize, string fileName)
{
    int permutation[n];
    int batches = 1000/batchSize;
        for(int j=0;j<n;j++)
        {
            permutation[j] = j+1;
        }

        int avgSum = 0;

        int bestRouteLength = INT_MAX;
        int globalBestPerm[n] = {0};

        for(int p=0;p<batches;p++)
        {
            int randomLengths[batchSize] = {0};
            for(int q=0;q<batchSize;q++)
            {
                randomPermutate(permutation, n);
                int length = 0;
                for(int k=1;k<n;k++)
                {
                    length+=distances[permutation[k-1]-1][permutation[k]-1];
                }
                randomLengths[q] = length;

                if(length < bestRouteLength)
                {
                    bestRouteLength = length;
                    for(int t=0;t<n;t++)
                    {
                        globalBestPerm[t] = permutation[t];
                    }
                }
            }

            int minimum = *min_element(randomLengths, randomLengths + batchSize);
            avgSum+=minimum;
            cout << minimum << " ";
        }
        cout << "\n" << fixed << setprecision(2) << (float)avgSum / batches << "\n";

        if(batchSize == 1000)
        {
            ofstream out("bestFoundRoutes.txt", ios::app);

            out << fileName << "\n";
            for(int x : globalBestPerm)
            {
                out << x << " ";
            }
            out << "\n" << bestRouteLength << "\n\n";
        }
}

void Salesman()
{
    //a
    for(int i=0;i<N;i++)
    {
        string directory = "data/" + fileNames[i];
        vector<City> cities = readCountry(directory);
        vector<vector<int>> distances = calculateDistances(cities);
        int n = cities.size();

        evaluateMin(n, distances, 10, fileNames[i]);
    }

    cout << "\n\n";

    //b
    for(int i=0;i<N;i++)
    {
        string directory = "data/" + fileNames[i];
        vector<City> cities = readCountry(directory);
        vector<vector<int>> distances = calculateDistances(cities);
        int n = cities.size();

        evaluateMin(n, distances, 50, fileNames[i]);
    }

    cout << "\n\n";

    //c
    for(int i=0;i<N;i++)
    {
        string directory = "data/" + fileNames[i];
        vector<City> cities = readCountry(directory);
        vector<vector<int>> distances = calculateDistances(cities);
        int n = cities.size();

        evaluateMin(n, distances, 1000, fileNames[i]);
    }
}

int main()
{  
    ofstream("bestFoundRoutes.txt").close();
    srand(time(0));
    Salesman();
}
