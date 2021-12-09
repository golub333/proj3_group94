#include <iomanip>
#include "graph3.cpp"

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

    while (selection != "done")
    {
        cout << setfill(' ') << setw(41) << "Select Operation: " << endl;
        cout << setfill('-') << setw(81) << "-" << endl;
        cout << "1. Calculate average degrees of separation using BFS" << endl;
        cout << "2. Calculate average shortest path length using Dijkstra's" << endl;
        cout << "3. Find the minimum spanning tree around a given user's connections" << endl;
        cout << "Type any other input if finished" << endl;
        cin >> selection;
        cout << endl;
        
        if (selection == "1" || selection == "1.")
        {
            cout << "Would you like to:" << endl;
            cout << "1. Input vertices" << endl;
            cout << "2. Use a set of randomly generated vertices" << endl;
            int choice;
            cin >> choice;
            if (choice == 1) {
                cout << "Enter starting vertex between 1 and " << maxSize << ":" << endl;
                int source;
                cin >> source;
                cout << "Enter destination vertex between 1 and " << maxSize << ":" << endl;
                int dest;
                cin >> dest;
                pair<double, int> degOfSep = twitterUsers.specifiedUsersBFS(source, dest);
                if (degOfSep.second == INT_MAX) {
                    cout << "No path found between vertices" << endl;
                }
                else {
                    cout << "Target vertices are " << degOfSep.second << " nodes apart" << endl;
                }
                cout << "The average degrees of separation found using BFS is: " << degOfSep.first << endl;
            }
            else {
                pair<double, int> degOfSep = twitterUsers.degreeSeparationBFS();
                cout << "The average degrees of separation found using BFS is: " << degOfSep.first << endl;
                cout << "The maximum degrees of separation found using BFS is: " << degOfSep.second << endl;
            }
        }
        else if (selection == "2" || selection == "2.")
        {
            cout << "Would you like to:" << endl;
            cout << "1. Input vertices" << endl;
            cout << "2. Use a set of randomly generated vertices" << endl;
            int choice;
            cin >> choice;
            if (choice == 1) {
                cout << "Enter starting vertex between 1 and " << maxSize << ":" << endl;
                int source;
                cin >> source;
                cout << "Enter destination vertex between 1 and " << maxSize << ":" << endl;
                int dest;
                cin >> dest;
                pair<double, int> degOfSep = twitterUsers.specifiedUsersDijkstra(source, dest);
                if (degOfSep.second == INT_MAX) {
                    cout << "No path found between vertices" << endl;
                }
                else {
                    cout << "Target vertices have a path which is " << degOfSep.second << " km long" << endl;
                }
                cout << "The average degrees distance found using Dijkstra's is: " << degOfSep.first << endl;
            }
            else {
                pair<double, int> degOfSep = twitterUsers.degreeSeparationDijkstra();
                cout << "The average distance found using Dijkstra's is: " << degOfSep.first << " km" << endl;
                cout << "The maximum distance found using Dijkstra's is: " << degOfSep.second << " km" << endl;
            } 
        }
        else if (selection == "3" || selection == "3.")
        {
            int user = 0;
            cout << "Enter a number (no commas) for the user you want to find a MST around" << endl;
            cin >> user;
            cout << endl;
            //Complete functionality for MST
            twitterUsers.primMST(user);
        }
        else //User has indicated that they are finished
        {
            selection = "done";
        }
        cout << endl;
    }
}
