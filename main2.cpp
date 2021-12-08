#include <iomanip>
#include "graph2.cpp"

using namespace std;

//twitter_dataset/1_million/

int main()
{
    int maxSize = 0;
    //Initialize the graph and get the data (for 1,000,000 coordinates)
    cout << setw(41) << "Welcome to our COP 3530 Project 3" << endl;
    cout << "Our program uses graph algorithms to solve problems on Twitter users' connections" << endl;
    cout << setfill('-') << setw(81) << "-" << endl;
    cout << "Indicate the number of users you wish to include in the graph - Max of 1 million:" << endl;
    cin >> maxSize;
    cout << endl;
    Graph twitterUsers(maxSize);

    //Begin user interaction with the program
    string selection = "-1";
    cout << setfill(' ') << setw(41) << "Select Operation: " << endl;
    cout << setfill('-') << setw(81) << "-" << endl;
    cout << "1. Calculate Avg. and Max. Degrees of separation using BFS" << endl;
    cout << "2. Calculate Avg. and Max. Degrees of separation using Dijkstra's" << endl;
    cout << "3. Find the minimum spanning tree around a given user's connections" << endl;
    cin >> selection;
    cout << endl;

    while (selection != "done")
    {
        if (selection == "1" || selection == "1.")
        {
            pair<int, int> degOfSep = twitterUsers.degreeSeparationBFS();
            cout << "The average degrees of separation found using BFS is: " << degOfSep.first << endl;
            cout << "The maximum degrees of separation found using BFS is: " << degOfSep.second << endl;
            selection = "done";
        }
        else if (selection == "2" || selection == "2.")
        {
            pair<int, int> degOfSep = twitterUsers.degreeSeparationDijkstra();
            cout << "The average degrees of separation found using Dijkstra's is: " << degOfSep.first << endl;
            cout << "The maximum degrees of separation found using Dijkstra's is: " << degOfSep.second << endl;
            selection = "done";
        }
        else if (selection == "3" || selection == "3.")
        {
            int user = 0;
            cout << "Enter a number (no commas) for the user you want to find a MST around" << endl;
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
