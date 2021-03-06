#include <unordered_map>
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
using namespace std;

class Graph
{
private:
    unordered_map<int, vector<pair<int, unsigned int>>> adjList;
    vector<pair<double, double>> coordinates;
public:
    int maxSize;
    Graph(int maxSize);
    void parseUsers(int& maxSize);
    void GetLocationData(int& maxSize);
    int Distance(int x, int y);
    void addEdge(int u, int v);
    bool checkValidity(unordered_map <int, bool>& inMST, int from, int to);
    void primMST(int from);
    pair<double, int> degreeSeparationBFS();
    pair<double, int> degreeSeparationDijkstra();
    vector<int> BFS(int source, int maxSize);
    vector<int> dijkstra(int source, int maxSize);
    pair<double, int> specifiedUsersBFS(int source, int dest);
    pair<double, int> specifiedUsersDijkstra(int source, int dest);

};

Graph::Graph(int maxSize)
{
    GetLocationData(maxSize); 
    parseUsers(maxSize);
    this->maxSize = maxSize;
}
//Takes in the file of locations and places into a vector called locations
void Graph::GetLocationData(int& maxSize)
{
    double conv = 0.017453292519943295; //pi/180 - needed to convert coordinates to radians - makes things easier in the distance function
    ifstream in("1_million_location.txt");

    if (in.is_open())
    {
        string line = "";
        int index = 0;
        coordinates.push_back(make_pair(0,0));
        while (in && index < maxSize)
        {
            getline(in, line);
            index++;
            unsigned int comma = line.find(',');
            string latString = line.substr(0, comma);
            string intString = line.substr(comma + 1);
            coordinates.push_back(make_pair(stod(latString) * conv, stod(intString) * conv));
            //c[index].first = (stod(latString) * conv);
            //c[index].second = (stod(intString) * conv);
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
void Graph::parseUsers(int& maxSize)
{
    string user;
    int userNum;
    int index = 0;
    string line;
    ifstream userFile("1_million_user.txt");
    if (userFile.is_open())
    {
        while (getline(userFile, line) && index < maxSize)
        {
            index++;
            bool moreVertices = true;
            while (moreVertices)
            {
                //To get the first user in the line that the rest connect to
                if (line.find(",") != std::string::npos)
                {
                    int position = line.find(",");
                    user = line.substr(0, position);
                    line = line.substr(position + 2);
                    userNum = stoi(user);
                    if (userNum <= maxSize)
                        addEdge(index, userNum);
                }
                //connects the last user of the line
                else
                {
                    userNum = stol(line);
                    if (userNum <= maxSize)
                        addEdge(index, userNum);
                    moreVertices = false;
                }
            }
        }
    }
}

//Connects two vertices and how far they are apart. Adds to adjacency list
void Graph::addEdge(int from, int to)
{
    int distance = Distance(from, to);//Change this to the distance function

    //Checks to see if the vertex(from) already exists. If it does, it adds the new connected vertex(to)
    if (adjList.find(from) != adjList.end())
        adjList.find(from)->second.push_back(make_pair(to, distance));

    //If the vertex(from) does not exist, it creates it and adds its connected vertex (to)
    else
    {
        vector <pair<int, unsigned int>> temp;
        temp.push_back(make_pair(to, distance));
        adjList.emplace(from, temp);
    }

    //If the connected vertex(to) does not exist, it is added
    if (adjList.find(to) == adjList.end())
    {
        vector <pair<int, unsigned int>> temp;
        temp.push_back(make_pair(-1, 0));
        adjList.emplace(to, temp);
    }

}

//Checks if the two vertices are valid for primMST
bool Graph::checkValidity(unordered_map <int, bool>& inMST, int from, int to)
{
    if (from == to)
        return false;
    if (inMST[from] == false && inMST[to] == false)
        return false;
    if (inMST[from] == true && inMST[to] == true)
        return false;

    return true;
}

void Graph::primMST(int from)
{
    unordered_map<int, bool> inMST;

    inMST.emplace(from, true);
    int edge_count = 0;
    int minCost = 0;
    int count = 0;

    while (count < this->adjList.size() - 1)
    {
        int min = INT_MAX;
        int f = -1;
        int to = -1;

        for (auto i = this->adjList.begin(); i != this->adjList.end(); i++)
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
    cout << "User " << from << " can reach " << edge_count << " users, spanning across a minimum distance of " << minCost  << " km" << endl;
}
//Uses a modified form of breadth first search to calculate distance of all nodes which can be reached from a 
//source node, with the edge weights all equal to one. Unreachable nodes will be left with a distance of INT_MAX.
vector<int> Graph::BFS(int source, int maxSize) {

    vector<int> visited(maxSize + 1, false);
    vector<int> distance(maxSize + 1, INT_MAX);

    queue<int> q;
    distance[source] = 0;
    visited[source] = true;
    q.push(source);

    //Iterates through queue, visiting each node which can be reached
    while (!q.empty()) {
        int tempVertex = q.front();
        q.pop();

        //Iterates through neighbors of given node
        for (int i = 0; i < adjList[tempVertex].size(); i++) {
            int temp = adjList[tempVertex][i].first;

            //If given node has not been visited, update its distance to be that of its predecessor + 1
            //and mark it visited, then add its neighbors to the queue
            if (temp != -1) {
                if (!visited[temp]) {
                    visited[temp] = true;
                    distance[temp] = distance[tempVertex] + 1;
                    q.push(temp);
                }
            }
        }
    }
    return distance;
}
//Calculates distance to all nodes from a given source node, where the edge weight is geographical distance. 
//Distance to any unreachable nodes will be left at INT_MAX.
vector<int> Graph::dijkstra(int source, int maxSize) {

    //Min heap containing pairs which store the distance to the given vertex and the vertex
    priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    vector<int> distance(maxSize + 1, INT_MAX);

    distance[source] = 0;
    pq.push(make_pair(0, source));

    //Iterates through min heap until it is empty
    while (!pq.empty()) {
        int tempVertex = pq.top().second;
        pq.pop();

        //Iterates through given vertex's neighbors
        for (pair<int, unsigned int> temp : adjList[tempVertex]) {

            int neighbor = temp.first;
            unsigned int weight = temp.second;

            //Updates distance of vertex if there is a shorter path found
            if (neighbor != -1) {
                if ((distance[tempVertex] + weight) < distance[neighbor]) {
                    distance[neighbor] = distance[tempVertex] + weight;
                    pq.push(make_pair(distance[neighbor], neighbor));
                }
            }
        }
    }
    return distance;
}
//First value in pair is average degrees of separation, second is largest distance found
pair<double, int> Graph::degreeSeparationBFS() {

    int tempLargest = 0;
    double average = 0;

    int numConnectedNodes = 0;
    int sum = 0;

    uniform_int_distribution<> d(1, maxSize);
    mt19937 gen;

    //Runs BFS on ten different randomly generated vertices within graph range
    for (int i = 0; i < 10; i++) {
        int tempRand = d(gen);
        while (adjList[tempRand][0].first == -1) {
            tempRand = d(gen);
        }
        
        vector<int> distances = BFS(tempRand, maxSize);

        //Sums the values of all distances calculated as well as the number of nodes which were reached in each BFS
        for (int temp : distances) {
            if (temp != INT_MAX && temp != 0) {
                sum += temp;
                numConnectedNodes++;
                //Stores the intest distance between 2 nodes found
                if (temp > tempLargest) {
                    tempLargest = temp;
                }
            }
        }
    }
    //Takes the average of all distances
    average = ((double)sum) / ((double)numConnectedNodes);

    return make_pair(average, tempLargest);
}
//First value in pair is average distance to users, second is largest distance found
pair<double, int> Graph::degreeSeparationDijkstra() {

    int tempLargest = 0;
    double average = 0;

    double numConnectedNodes = 1;
    long sum = 0;

    uniform_int_distribution<> d(1, maxSize);
    mt19937 gen;

    //Runs Dijkstra's on ten different randomly generated vertices within graph range
    for (int i = 0; i < 10; i++) {
        int tempRand = d(gen);
        while (adjList[tempRand][0].first == -1) {
            tempRand = d(gen);
        }
        vector<int> distances = dijkstra(tempRand, maxSize);

        //Calculates average at each step to avoid integer overflow
        for (int temp : distances) {
            if (temp != INT_MAX && temp != 0) {
                average = (numConnectedNodes - 1) / numConnectedNodes * average + temp / numConnectedNodes;
                numConnectedNodes++;

                if (temp > tempLargest) {
                    tempLargest = temp;
                }
            }
        }
    }
    return make_pair(average, tempLargest);
}
//First value in pair is average distance to users from source vertex, second is distance to dest
pair<double, int> Graph::specifiedUsersBFS(int source, int dest) {
    double average = 0;

    int numConnectedNodes = 0;
    int sum = 0;

    vector<int> distances = BFS(source, maxSize);

    //Sums the values of all distances calculated
    for (int temp : distances) {
        if (temp != INT_MAX && temp != 0) {
            sum += temp;
            numConnectedNodes++;
        }
    }
    int distanceToUser = distances[dest];
    average = ((double)sum) / ((double)numConnectedNodes);

    return make_pair(average, distanceToUser);
}
//First value in pair is average distance to users from source vertex, second is distance to dest
pair<double, int> Graph::specifiedUsersDijkstra(int source, int dest) {
    double average = 0;

    double numConnectedNodes = 1;
    int sum = 0;

    vector<int> distances = dijkstra(source, maxSize);

    //Sums the values of all distances calculated
    for (int temp : distances) {
        if (temp != INT_MAX && temp != 0) {
            average = (numConnectedNodes - 1) / numConnectedNodes * average + temp / numConnectedNodes;
            numConnectedNodes++;
        }
    }
    int distanceToUser = distances[dest];

    return make_pair(average, distanceToUser);
}
