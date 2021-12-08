#include <iomanip>
#include "graph.cpp"

using namespace std;

//twitter_dataset/1_million/

int main()
{
    //Initialize the graph and get the data (for 1,000,000 coordinates)
    Graph twitterUsers;

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