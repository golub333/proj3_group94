#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

void GetLocationData(vector<pair<double,double>>& c)
{
    double conv = 0.017453292519943295; //pi/180 - needed to convert coordinates to radians - makes things easier in the distance function
    ifstream in("twitter_dataset/1_million/1_million_location.txt");

    if (in.is_open())
    {
        string line = "";
        int index = 0;
        while(in)
        {
            getline(in, line);
            index++;
            unsigned int comma = line.find(',');
            string latString = line.substr(0,comma);
            string longString = line.substr(comma+1);
            c[index].first = (stod(latString)*conv);
            c[index].second = (stod(longString)*conv);
        }
    }
    else
        cout << "Could not open file for Location Data" << endl;
}

int distance(vector<pair<double,double>>& c, int x, int y)
{
    double R = 6371.0; //approximation of the earth's avg radius
    double firstTerm = sin((c[y].first-c[x].first)/2)*sin((c[y].first-c[x].first)/2);
    double secondTerm = 1-firstTerm-(sin((c[y].first+c[x].first)/2)*sin((c[y].first+c[x].first)/2));
    secondTerm *= sin((c[y].second-c[x].second)/2)*sin((c[y].second-c[x].second)/2);
    double dist = sqrt(firstTerm+secondTerm);
    dist = 2*R*asin(dist);
    return round(dist);
}

int main()
{
    //Initialize the graph and get the data (for 1,000,000 coordinates)
    Graph twitterUsers;
    vector<pair<double,double>> coordinates(1000000);
    GetLocationData(coordinates);
    //Get the graph data - will need to pass in the coordinates graph to call the distance formula

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
            pair<int,int> degOfSep = degreeSeparationBFS(twitterUsers);
            cout << "The average degrees of separation found using BFS is: " << degOfSep.first << endl;
            cout << "The maximum degrees of separation found using BFS is: " << degOfSep.second << endl;
            selection = "done";
        }
        else if (selection == "2" || selection == "2.")
        {
            pair<int,int> degOfSep = degreeSeparationDijkstra(twitterUsers);
            cout << "The average degrees of separation found using Dijkstra's is: " << degOfSep.first << endl;
            cout << "The maximum degrees of separation found using Dijkstra's is: " << degOfSep.second << endl;
            selection = "done";
        }
        else if (selection == "3" || selection == "3.")
        {
            int user = 0;
            cout << "Enter a number 1-1,000,000 (no commas) for the user you want to find a MST around" << endl;
            cin >> user;
            cout << endl;
            //Complete functionality for MST
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