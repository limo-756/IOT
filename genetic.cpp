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
        cout << "ERROR : IN randVar(int a,int b) IN RANDOMIZE.cpp" << endl;
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
};
class Topology {
    int numberOfpoints;
    int windowXCoord;
    int windowYCoord;
    int numberOfBaseStation;
    const int MAX_RANGE = 30;
    vector< pair<int,int> > sensors;
    unordered_set< pair<int,int> > points;
public:
    vector< vector<int> > matrix;
    Topology(int numberOfpoints,int numberOfBaseStation,int windowXCoord,int windowYCoord)
    {
        this.numberOfpoints = numberOfpoints;
        this.numberOfBaseStation = numberOfBaseStation;
        this.windowXCoord = windowXCoord;
        this.windowYCoord = windowYCoord;
        generateRandomPoints();
        for (int i = 0; i < numberOfpoints; ++i)
            matrix.push_back(vector<int>());
        adjacencyMatrixCreation();
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
        }
    }
    void generateRandomPoints()
    {
        for (int i = 0; i < numberOfpoints; ++i)
        {
            do
            {
                int x = randVar(0,windowXCoord);
                int y = randVar(0,windowYCoord);
            }while(points.find(make_pair(x,y)) != points.end());
            points.insert(make_pair(x,y));
            int range = rand()%MAX_RANGE;
            sensors.push_back(Node(make_pair(x,y),range));
        }
    }
};
class Cromosome
{
    Topology *center;
    unordered_map< pair<int,bool> > bag;
    vector< int > baseStations;
    double cost;
    const int MAX_MUTATIONS = 3;
public:
    Cromosome(Topology* center):center(center) {}
    Cromosome(){}
    Cromosome crossOver(Cromosome &parent1,Cromosome &parent2)
    {
        Cromosome res = new Cromosome();
        res.center = parent1.getCenter();
        int num = randVar(0 ,size/2);
        res.copy(num,parent1);
        res.copy(size,parent2);
        return res;
    }
    Topology* getCenter()
    {
        return center;
    }
    void copy(int size,Cromosome &parent)
    {
        for (int i = 0; i < size; ++i)
        {
            if(bag.find(parent.baseStations[i]) == bag.end())
            {
                bag.insert(parent.baseStations[i]);
                baseStations.push_back(parent.baseStations[i]);
            }
        }
    }
    void mutation(Cromosome &citizen)
    {
        int num = randVar(0,MAX_MUTATIONS)+1;
        for (int i = 0; i < num; ++i)
        {
            int index = randVar(0,baseStations.size());
            while(true)
            {
                int newIndex = randVar(0,center->getNumberOfPoints());
                if(bag.find(index) == bag.end())
                    break;
            }
            bag.remove(baseStations[index]);
            bag.insert(newIndex);
            baseStations[index] = newIndex;
        }
    }
    double getCost()
    {
        return cost;
    }
    double calCost()
    {
        cost = 0;
        double dist = 0;
        for(auto station : baseStations)
        {
            for (int i = 0; i < center->getNumberOfPoints(); ++i)
            {
                if(center->matrix[station][i])
                    dist = dist + center->dist(station,i);
            }
        }
        cost = (1.0*dist/INT_MAX);
        return cost;
    }
}
void genetic()
{
    Topology tj = new Topology(50,5,100,100);
}
int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
    return 0;
}