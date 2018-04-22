#include "/home/anurag/desktop/program/codeForces/templates/MYLIB.cpp"
// #include "/home/anurag/desktop/program/codeForces/templates/RANDOMIZE.cpp"
// #include "/home/anurag/desktop/program/codeForces/templates/TEST.cpp"
#include <bits/stdc++.h>
using namespace std;
template<typename T>
T randVar(T n)
{
    return rand()%n;
}
template<typename T,typename P>
T randVar(T a,P b)
{
    if(b < a)
    {
        cout << "ERROR : IN randVar(int a,int b)" << endl;
        cout << "a > b" << endl;
        exit(0);
    }
    if(a < 0)
    {
        if(b-a == 0)
            return a + randVar(-a);
        else
            return randVar(b - a + 1) + a;
    }
    if(a == 0 && b == 0)
        return 0;
    if(b-a == 0)
        return a;
    else
        return randVar(b - a + 1) + a;
}
class Node {
private:
    double range;
    pair<int,int> coord;
public:
    Node(pair<int,int> coord,double range) : coord(coord) , range(range) {}
    int getXCoord()
    {
        return coord.first;
    }
    int getYCoord()
    {
        return coord.second;
    }
    pair<int,int> getCoord()
    {
        return coord;
    }
    double getRange()
    {
        return range;
    }
    double setRange(double range)
    {
        this->range = range;
    }
    void print()
    {
        cout << "(" << coord.first << "," << coord.second << ")" << " range : " << range << endl;
    }
};
class Topology {
    int numberOfpoints;
    int windowXCoord;
    int windowYCoord;
    int numberOfBaseStation;
    const int MIN_RANGE = 20;
    const int MAX_RANGE = 50;
    set< pair<int,int> > points;
public:
    vector< Node > sensors;
    vector< vector<int> > matrix;
    Topology(int numberOfpoints,int numberOfBaseStation,vector<Node> sensorCoord)
    {
        this->numberOfpoints = numberOfpoints;
        this->numberOfBaseStation = numberOfBaseStation;
        this->sensors = sensorCoord;
        for (int i = 0; i < numberOfpoints; ++i)
        {
            matrix.push_back(vector<int>());
            for (int j = 0; j < numberOfpoints; ++j)
                matrix[i].push_back(0);
        }
        adjacencyMatrixCreation();
        for (int i = 0; i < numberOfpoints; ++i)
        {
            cout << (i+1);
            sensors[i].print();
        }
    }
    Topology(int numberOfpoints,int numberOfBaseStation,int windowXCoord,int windowYCoord)
    {
        this->numberOfpoints = numberOfpoints;
        this->numberOfBaseStation = numberOfBaseStation;
        this->windowXCoord = windowXCoord;
        this->windowYCoord = windowYCoord;
        generateRandomPoints();
        for (int i = 0; i < numberOfpoints; ++i)
        {
            matrix.push_back(vector<int>());
            for (int j = 0; j < numberOfpoints; ++j)
                matrix[i].push_back(0);
        }
        adjacencyMatrixCreation();
        for (int i = 0; i < numberOfpoints; ++i)
        {
            cout << (i+1);
            sensors[i].print();
        }
    }
    int getNumberOfBaseStation()
    {
        return numberOfBaseStation;
    }
    int getNumberOfPoints()
    {
        return numberOfpoints;
    }
    bool inRange(int x)
    {
        return (0 <= x && x < numberOfpoints);
    }
    double dist(int i,int j)
    {
        if(!inRange(i) || !inRange(j))
        {
            cout << "Error : in topology in dist function" << endl;
            cout << "Index i and j out of bound" << endl;
            exit(0);
        }
        double res = sqrt(pow(sensors[i].getXCoord() - sensors[j].getXCoord(),2) + pow(sensors[i].getYCoord() - sensors[j].getYCoord(),2));
        return res;
    }
    void adjacencyMatrixCreation()
    {
        for (int i = 0; i < numberOfpoints ; ++i)
        {
            for (int j = i+1; j < numberOfpoints; ++j)
            {
                if(dist(i,j) <= sensors[i].getRange())
                    matrix[i][j] = 1;
                if(dist(i,j) <= sensors[j].getRange())
                    matrix[j][i] = 1;
            }
            matrix[i][i] = 1;
        }
    }
    void generateRandomPoints()
    {
        for (int i = 0; i < numberOfpoints; ++i)
        {
            int x,y;
            do
            {
                x = randVar(0,windowXCoord);
                y = randVar(0,windowYCoord);
            }while(points.find(make_pair(x,y)) != points.end());
            points.insert(make_pair(x,y));
            int range = randVar(MIN_RANGE,MAX_RANGE);
            sensors.push_back(Node(make_pair(x,y),range));
        }
    }
};
const int MAX_BASE_STATIONS = 1;
class Cromosome
{
    Topology *center;
    unordered_set<int> bag;
    vector< int > baseStations;
    long long int cost;
    const int MAX_MUTATIONS = 1;
    /**
     * Base stations should be more than 1
     */
public:
    Cromosome(Topology* center):center(center)
    {
        while(bag.size() != MAX_BASE_STATIONS)
        {
            int newIndex = randVar(0,center->getNumberOfPoints() - 1);
            if(bag.find(newIndex) == bag.end())
            {
                bag.insert(newIndex);
                baseStations.push_back(newIndex);
            }
        }
        cost = 1e18;
    }
    Cromosome()
    {
        cost = 1e18;
    }
    void operator=(Cromosome &otherCrom)
    {
        center = otherCrom.center;
        cost = otherCrom.getCost();
        bag.clear();
        baseStations.clear();
        for(auto i : otherCrom.baseStations)
        {
            bag.insert(i);
            baseStations.push_back(i);
        }
    }
    Topology* getCenter()
    {
        return center;
    }
    void setCenter(Topology *newCenter)
    {
        center = newCenter;
    }
    void copy(int size,Cromosome *parent)
    {
        for (int i = 0; i < parent->baseStations.size() && bag.size() <= size; ++i)
        {
            if(bag.find(parent->baseStations[i]) == bag.end())
            {
                bag.insert(parent->baseStations[i]);
                baseStations.push_back(parent->baseStations[i]);
            }
        }
    }
    void mutation()
    {
        if(getNumberOfBaseStation() == 1)
        {
            int index;
            do
            {
                index = randVar(0,center->getNumberOfPoints() - 1);
            } while (bag.find(index) == bag.end());
            bag.erase(baseStations[0]);
            bag.insert(index);
            baseStations[0] = index;
        }
        else
        {
            int num = randVar(1,MAX_MUTATIONS);
            for (int i = 0; i < num; ++i)
            {
                int index = randVar(0,baseStations.size() - 1);
                int newIndex;
                while(true)
                {
                    newIndex = randVar(0,center->getNumberOfPoints() - 1);
                    if(bag.find(newIndex) == bag.end())
                        break;
                }
                bag.erase(baseStations[index]);
                bag.insert(newIndex);
                baseStations[index] = newIndex;
            }
        }
    }
    long long int getCost()
    {
        return cost;
    }
    long long int evaluate()
    {
        cost = 0;
        long long int distances[center->getNumberOfPoints()];
        for(int i = 0;i < center->getNumberOfPoints();i++)
            distances[i] = INT_MAX;
        for(auto station : baseStations)
        {
            for (int i = 0; i < center->getNumberOfPoints(); ++i)
            {
                if(center->matrix[station][i])
                {
                    long long int dist = center->dist(station,i);
                    distances[i] = min(dist,distances[i]);
                }
            }
        }
        long long int dist = 0;
        for(int i = 0;i < center->getNumberOfPoints();i++)
            dist = dist + distances[i];
        cost = dist;
        return cost;
    }
    int getNumberOfBaseStation()
    {
        return center->getNumberOfBaseStation();
    }
    void print()
    {
        cout << "PRINTING THE CROMOSOME" << endl;
        cout << "cost is " << cost << endl;
        for(auto station : baseStations)
        {
            cout << station << " ";
            center->sensors[station].print();
        }
        cout << endl;
    }
};
Cromosome* crossOver(Cromosome *parent1,Cromosome *parent2)
{
    Cromosome *res = new Cromosome();
    res->setCenter(parent1->getCenter());
    if(parent1->getNumberOfBaseStation())
    {
        int num = randVar(0,1);
        if(num == 0)
            res->copy(1,parent1);
        else
            res->copy(1,parent2);
    }
    else
    {
        int num = randVar(1 ,MAX_BASE_STATIONS/2);
        res->copy(num,parent1);
        res->copy(MAX_BASE_STATIONS,parent2);
    }
    return res;
}
bool comp(Cromosome *a,Cromosome *b)
{
    return a->getCost() < b->getCost();
}
void genetic(int populationSize,int numberOfRounds)
{
    cout << "Genetic Algo Started" << endl;
    // Topology *tj = new Topology(50,5,100,100);
    vector<Node> vec;
    vec.push_back(Node(make_pair(0,0),10));
    vec.push_back(Node(make_pair(10,0),10));
    vec.push_back(Node(make_pair(0,10),10));
    vec.push_back(Node(make_pair(-10,0),10));
    vec.push_back(Node(make_pair(0,-10),10));
    Topology *tj = new Topology(5,1,vec);
    vector< Cromosome* > population;
    for (int i = 0; i < populationSize; ++i)
    {
        population.push_back(new Cromosome(tj));
        // population[i]->print();
    }
    Cromosome best(tj);
    best.evaluate();
    best.print();
    for (int i = 0; i < numberOfRounds; ++i)
    {
        cout << "Round Number : " << (i+1) << endl;
        for (int i = 0; i < populationSize; ++i)
            population[i]->evaluate();
        sort(population.begin(), population.end(), comp);
        if(best.getCost() > population[0]->getCost())
            best = (*population[0]);
        int best30 = (populationSize*30)/100;
        int random20 = (populationSize*20)/100;
        vector< Cromosome* > newPopulation;
        for (int i = 0; i < best30; ++i)
            newPopulation.push_back(population[i]);
        for (int i = 0; i < random20; ++i)
        {
            int index = randVar(0,populationSize - 1);
            newPopulation.push_back(population[index]);
        }
        while(newPopulation.size() != populationSize)
        {
            int index1 = randVar(0,newPopulation.size() - 1);
            int index2 = randVar(0,newPopulation.size() - 1);
            newPopulation.push_back( crossOver(newPopulation[index1],newPopulation[index2]) );
            int index = randVar(0,newPopulation.size() - 1);
            newPopulation[index]->mutation();
        }
        population.clear();
        population = newPopulation;
        best.print();
    }
}
int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
    cout << "Starting Program" << endl;
    genetic(5,5);
    return 0;
}