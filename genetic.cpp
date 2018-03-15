#include <bits/stdc++.h>
using namespace std;
class topology {
    int numberOfpoints;
    int windowXCoord;
    int windowYCoord;
    vector< pair<int,int> > sensors;
    unordered_set< pair<int,int> > points;
public:
    topology(int numberOfpoints,int windowXCoord,int windowYCoord)
    {
        this.numberOfpoints = numberOfpoints;
        this.windowXCoord = windowXCoord;
        this.windowYCoord = windowYCoord;
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
            sensors.push_back(make_pair(x,y));
        }
    }
};
int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
    return 0;
}