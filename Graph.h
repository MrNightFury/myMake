#ifndef __GRAPH_H
#define __GRAPG_H
#include <vector>
#include <string>
using namespace std;

struct Task {
    string name;
    vector<string> commands;
    vector<int> from;
    vector<int> to;
    vector<string> lastHashes;
};
string to_string (Task task);

class Graph {
    vector<Task*> tasks;
    vector<vector<int>> from;
    vector<vector<int>> to;

    public:
        void addTask (string name);
        void addLine (string name_1, string name_2);
        bool isExist (string name);
        int getIndex (string name);
        void show ();
        void addCommand (string name, string command);
        bool isEmpty();
        vector<string> getCommands (string name);

        Task* getTask (int id);
        int getTaskCount () {return tasks.size();};
};

#endif