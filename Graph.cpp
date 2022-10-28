#include "Graph.h"
#include <iostream>
using namespace std;

string to_string (Task task) {
    string buffer = task.name + ": " + to_string(task.commands.size()) + " commands\nfrom:";
    for (int i = 0; i < task.from.size(); i++) {
        buffer += " " + to_string (task.from[i]);
    }
    buffer += "\nto:";
    for (int i = 0; i < task.to.size(); i++) {
        buffer += " " + to_string (task.to[i]);
    }
    return buffer;
}

void Graph::addTask (string name) {
    if (!isExist(name)) {
        tasks.push_back (new Task {name, vector<string>(), vector<int>(), vector<int>(), vector<string>()});
        //Outdated
        from.push_back (vector<int>());
        to.push_back (vector<int>());
    }
}

void Graph::addLine (string task, string subtask) {
    int taskI = getIndex(task);
    int subtaskI = getIndex(subtask);
    if (taskI == -1) {
        addTask (task);
        taskI = getIndex(task);
    }
    if (subtaskI == -1) {
        addTask (subtask);
        subtaskI = getIndex(subtask);
    }
    tasks[taskI]->from.push_back(subtaskI);
    tasks[subtaskI]->to.push_back(taskI);
    tasks[taskI]->lastHashes.push_back ("-");
    //outd
    // from[taskI].push_back(subtaskI);
    to[subtaskI].push_back(taskI);
}

//upd
bool Graph::isExist (string name) {
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i]->name == name) {
            return true;
        }
    }
    return false;
}

//upd
int Graph::getIndex (string name) {
    if (name == "_first_task_signal_") {
        return 0;
    }
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i]->name == name) {
            return i;
        }
    }
    return -1;
}

//upd
void Graph::show() {
    for (int i = 0; i < tasks.size(); i++) {
        cout << '*' << tasks[i]->name << "*\n";
    }
    cout << "----------\n";
    for (int i = 0; i < tasks.size(); i++) {
        if (!tasks[i]->from.empty()) {
            for (int j = 0; j < tasks[i]->from.size(); j++) {
                cout << tasks[i]->name << '-' << tasks[tasks[i]->from[j]]->name << '\n';
            }
        }
    }
}

//upd
void Graph::addCommand (string name, string command) {
    int index = getIndex (name);
    if (index != -1) {
        tasks[index]->commands.push_back(command);
    } else {
        cout << "Command can't be added [" << name << '|' << command << "]\n";
    }
}

//upd
bool Graph::isEmpty() {
    return tasks.empty();
}

//upd
vector<string> Graph::getCommands (string name) {
    return tasks[getIndex(name)]->commands;
}

//upd
Task* Graph::getTask (int id) {
    return (id != -1 ? tasks[id] : nullptr);
}