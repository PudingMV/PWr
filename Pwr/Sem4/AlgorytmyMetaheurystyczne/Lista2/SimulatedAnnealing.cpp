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

bool checkProbability(double Temperature, vector<vector<int>> distances, int startingSolution[], int newSolution[], int n)
{
    int delta = calculateRoute(distances, startingSolution, n) - calculateRoute(distances, newSolution, n);

    double P = exp(delta / Temperature);

    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<> dist(0.0, 1.0);

    double r = dist(gen);

    return r < P;
}

void SimulatedAnnealing(float startingTemperature, float endTemperature, float tempDecay, int eras, vector<City> cities)
{
    vector<vector<int>> distances = calculateDistances(cities);
    int n = cities.size();

    int startingSolution[n];
    for(int j=0;j<n;j++)
    {
        startingSolution[j] = j+1;
    }
    randomPermutate(startingSolution, n);

    while(startingTemperature > endTemperature)
    {
        for(int i = 0; i < eras; i++)
        {
            int newSolution[n];
            assignArray(startingSolution, newSolution, n);

            getNeighbourhooded(newSolution, n);

            if(calculateRoute(distances, startingSolution, n) > calculateRoute(distances, newSolution, n))
            {
                assignArray(newSolution, startingSolution, n);
            }
            else if(checkProbability(startingTemperature, distances, startingSolution, newSolution, n))
            {
                assignArray(newSolution, startingSolution, n);
            }
        }

        startingTemperature *= tempDecay;
    }

    ofstream out("bestFoundRoutes.txt", ios::app);

    for(int i = 0; i < n; i++)
    {
        out << cities[startingSolution[i] - 1].x << " " << cities[startingSolution[i] - 1].y << " ";
        cout << cities[startingSolution[i] - 1].x << " " << cities[startingSolution[i] - 1].y << " ";
    }

    out << calculateRoute(distances, startingSolution, n) << " " << n << "\n";
    cout << calculateRoute(distances, startingSolution, n) << " " << n << "\n";
}

void Solution(float startingTemperature, float endTemperature, float tempDecay, int eras)
{
    for(int i=0;i<N;i++)
    {
        string directory = "data/" + fileNames[i];
        vector<City> cities = readCountry(directory);

        SimulatedAnnealing(startingTemperature, endTemperature, tempDecay, eras, cities);

        cout << "\n\n";
    }

}

int main()
{
    ofstream("bestFoundRoutes.txt").close();
    srand(time(0));
    Solution(1000.0, 0.01, 0.995, 100);
}