
#include <iostream>
#include <map>
#include <iterator>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

//twitter_dataset/1_million/
class Graph
{
private:
    map<long, vector<pair<long, unsigned int>>> adjList;
    vector<pair<double, double>> coordinates;
public:
    Graph();
    void parseUsers();
    void GetLocationData();
    int Distance(int x, int y);
    void addEdge(map <long, vector<pair<long, unsigned int>>> &adjList, long u, long v);
    bool checkValidity(map <long, bool>& inMST, long from, long to);
    void primMST(long from);
};

Graph::Graph()
{
    //GetLocationData(); 
    //parseUsers();
}
//Takes in the file of locations and places into a vector called locations
void Graph::GetLocationData()
{
    double conv = 0.017453292519943295; //pi/180 - needed to convert coordinates to radians - makes things easier in the distance function
    ifstream in("1_million_location.txt");

    if (in.is_open())
    {
        string line = "";
        int index = 0;
        while (in)
        {
            getline(in, line);
            index++;
            unsigned int comma = line.find(',');
            string latString = line.substr(0, comma);
            string longString = line.substr(comma + 1);
            this->coordinates.push_back(make_pair(stod(latString) * conv, stod(longString) * conv));
            //c[index].first = (stod(latString) * conv);
            //c[index].second = (stod(longString) * conv);
        }
    }
    else
        cout << "Could not open file for Location Data" << endl;
}
int Graph::Distance(int x, int y)
{
    
    double R = 6371.0; //approximation of the earth's avg radius
    double firstTerm = sin((this->coordinates[y].first - this->coordinates[x].first) / 2) * sin((this->coordinates[y].first - this->coordinates[x].first) / 2);
    double secondTerm = 1 - firstTerm - (sin((this->coordinates[y].first + this->coordinates[x].first) / 2) * sin((this->coordinates[y].first + this->coordinates[x].first) / 2));
    secondTerm *= sin((this->coordinates[y].second - this->coordinates[x].second) / 2) * sin((this->coordinates[y].second - this->coordinates[x].second) / 2);
    double dist = sqrt(firstTerm + secondTerm);
    dist = 2 * R * asin(dist);
    return round(dist);
}
//Takes in the file of users. Parses each line and connects the users in an adjacency list (which is a map of vectors)
void Graph::parseUsers()
{
    string user;
    long userNum;
    long firstUser;
    
    string line;
    ifstream userFile("1_million_user.txt");
    if (userFile.is_open())
    {
        while (getline(userFile, line))
        {
            long count = 0;
            bool moreVertices = true;
            while (moreVertices)
            {
                //To get the first user in the line that the rest connect to
                if (line.find(",") != std::string::npos && count == 0)
                {
                    int position = line.find(",");
                    user = line.substr(0, position);
                    line = line.substr(position + 2);
                    firstUser = stol(user);
                    count++;
                }
                //connects the rest of the users to the first user
                else if (line.find(",") != std::string::npos)
                {
                    int position = line.find(",");
                    user = line.substr(0, position);
                    line = line.substr(position + 2);
                    userNum = stol(user);
                    addEdge(this->adjList, firstUser, userNum);
                }
                //connects the last user of the line
                else
                {
                    userNum = stol(line);
                    addEdge(this->adjList, firstUser, userNum);
                    moreVertices = false;
                }
            }
        }
    }
}

//Connects two vertices and how far they are apart. Adds to adjacency list
void Graph::addEdge(map <long, vector<pair<long, unsigned int>>> &adjList, long from, long to)
{
    int distance = Distance(from, to);//Change this to the distance function

    //Checks to see if the vertex(from) already exists. If it does, it adds the new connected vertex(to)
    if (adjList.find(from) != adjList.end())
        adjList.find(from)->second.push_back(make_pair(to, distance));

    //If the vertex(from) does not exist, it creates it and adds its connected vertex (to)
    else
    {
        vector <pair<long, unsigned int>> temp;
        temp.push_back(make_pair(to, distance));
        adjList.emplace(from, temp);
    }

    //If the connected vertex(to) does not exist, it is added
    if (adjList.find(to) == adjList.end())
    {
        vector <pair<long, unsigned int>> temp;
        temp.push_back(make_pair(-1, 0));
        adjList.emplace(to, temp);
    }
        
}

bool Graph::checkValidity(map <long, bool> &inMST, long from, long to)
{
    if (from == to)
        return false;
    if (inMST[from] == false && inMST[to] == false)
        return false;
    if (inMST[from] == true && inMST[to] == true)
        return false;

    return true;
}
void Graph::primMST(long from)
{
    map<long, bool> inMST;
    
    inMST.emplace(from, true);
    int edge_count = 0;
    int minCost = 0;
    int count = 0;

    while (count < this->adjList.size() - 1)
    {
        int min = INT_MAX;
        long f = -1;
        long to = -1;

        for (auto i = this->adjList.find(from); i != this->adjList.end(); i++)
        {
            for (int j = 0; j < i->second.size(); j++)
            { 
                if (i->second[j].second < min && i->second[j].second != 0)
                {
                    if (checkValidity(inMST, i->first, i->second[j].first))
                    {
                        min = i->second[j].second;
                        f = i->first;
                        to = i->second[j].first;
                    }
                }
            }
        }
        if (f != -1 && to != -1)
        {
            edge_count++;
            minCost += min;
            inMST.emplace(f, true);
            inMST.emplace(to, true);
        }
        count++;
    }
    cout << "User " << from << " can reach " << edge_count << " users, spanning across a minimum distance of " << minCost << endl;
}
int main()
{
    //Initialize the graph and get the data (for 1,000,000 coordinates)
    Graph twitterUsers;
    //vector<pair<double, double>> coordinates(1000000);
    //GetLocationData(coordinates);
    //Get the graph data - will need to pass in the coordinates graph to call the distance formula
    twitterUsers.GetLocationData();
    twitterUsers.parseUsers();

    //Begin user interaction with the program
    string selection = "-1";
    cout << setw(41) << "Welcome to our COP 3530 Project 3" << endl;
    cout << "Our program uses graph algorithms to solve problems on Twitter users' connections" << endl;
    cout << setfill('-') << setw(82) << endl;
    cout << setw(41) << "Select Operation: " << endl;
    cout << setfill('-') << setw(82) << endl;
    cout << "1. Calculate Avg. and Max. Degrees of separation using BFS" << endl;
    cout << "2. Calculate Avg. and Max. Degrees of separation using Dijkstra's" << endl;
    cout << "3. Find the minimum spanning tree around a given user's connections" << endl;
    cin >> selection;
    cout << endl;

    while (selection != "done")
    {
        if (selection == "1" || selection == "1.")
        {
            //pair<int, int> degOfSep = degreeSeparationBFS(twitterUsers);
            //cout << "The average degrees of separation found using BFS is: " << degOfSep.first << endl;
            //cout << "The maximum degrees of separation found using BFS is: " << degOfSep.second << endl;
            selection = "done";
        }
        else if (selection == "2" || selection == "2.")
        {
            //pair<int, int> degOfSep = degreeSeparationDijkstra(twitterUsers);
            //cout << "The average degrees of separation found using Dijkstra's is: " << degOfSep.first << endl;
            //cout << "The maximum degrees of separation found using Dijkstra's is: " << degOfSep.second << endl;
            selection = "done";
        }
        else if (selection == "3" || selection == "3.")
        {
            int user = 0;
            cout << "Enter a number 1-1,000,000 (no commas) for the user you want to find a MST around" << endl;
            cin >> user;
            cout << endl;
            //Complete functionality for MST
            twitterUsers.primMST(user);
            selection = "done";
        }
        else //invalid input. Ask the user again
        {
            cout << "Invalid input. Enter 1, 2, or 3 corresponding to the operation you wish to select" << endl;
            cout << setfill('-') << setw(82) << endl;
            cout << "1. Calculate Avg. and Max. Degrees of separation using BFS" << endl;
            cout << "2. Calculate Avg. and Max. Degrees of separation using Dijkstra's" << endl;
            cout << "3. Find the minimum spanning tree around a given user's connections" << endl;
            cin >> selection;
            cout << endl;
        }
    }
}

