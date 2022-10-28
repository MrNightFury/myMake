#include "Graph.h"
#include <iostream>
using namespace std;

string to_string (Task task) {
    string buffer = to_string(task.id) + ' ' + task.name + ": " + to_string(task.commands.size()) + " commands\nfrom:";
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
        tasks.push_back (new Task {(int)tasks.size(), name, vector<string>(), vector<int>(), vector<int>(), vector<string>()});
        //Outdated
        names.push_back (name);
        commands.push_back (vector<string>());
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
    from[taskI].push_back(subtaskI);
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

void Graph::show() {
    /*for (int i = 0; i < names.size(); i++) {
        cout << names[i] << ':' << endl;
        if (!commands[i].empty()) {
            cout << "Commands:\n";
            for (int j = 0; j < commands[i].size(); j++) {
                cout << '$' << commands[i][j] << '\n';
            }
        }
    }*/

    // for (int i = 0; i < tasks.size(); i++) {
    //     cout << to_string (tasks[i]) << endl;
    // }
    
    for (int i = 0; i < names.size(); i++) {
        cout << '*' << names[i] << "*\n";
    }
    cout << "----------\n";
    for (int i = 0; i < names.size(); i++) {
        if (!from[i].empty()) {
            for (int j = 0; j < from[i].size(); j++) {
                cout << names[i] << '-' << names[from[i][j]] << '\n';
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

//not ready
/*
void Graph::delTask (string name) {
    int index = getIndex (name);
    for (int i = 0; i < tasks[index].to.size(); i++) {
        for (int j = 0; j < tasks[tasks[index].to[i]].from.size(); j++) {
            if (tasks[tasks[index].to[i]].from[j] == index) {
                tasks[tasks[index].to[i]].from.erase (tasks[tasks[index].to[i]].from.begin() + j);
            }
        }
    }
    tasks.erase (tasks.begin() + index);
    //outd
    vector<int> vect = to[index];
    for (int i = 0; i < vect.size(); i++) {
        for (int j = 0; j < from[vect[i]].size(); j++) {
            if (from[vect[i]][j] == index) {
                from[vect[i]].erase (from[vect[i]].begin() + j);
            }
        }
    }
    for (int i = 0; i < to.size(); i++) {
        for (int j = 0; j < to[i].size(); j++) {
            if (to[i][j] > index) {
                to[i][j]--;
            }
        }
        for (int j = 0; j < from[i].size(); j++) {
            if (from[i][j] > index) {
                from[i][j]--;
            }
        }
    }
    names.erase (names.begin() + index);
    commands.erase (commands.begin() + index);
    from.erase (from.begin() + index);
    to.erase (to.begin() + index);
}*/

bool Graph::isEmpty() {
    return names.empty();
}

/*
vector<Task> Graph::getReadyTasks() {
    vector<Task> buffer;
    for (int i = 0; i < names.size(); i++) {
        if (from[i].empty()) {
            buffer.push_back (Task {i, names[i], commands[i], from[i], to[i], vector<string>()});
            delTask (names[i]);
        }
    }
    return buffer;
}*/

vector<string> Graph::getCommands (string name) {
    cout << commands.size() << '*' << getIndex(name) << '*' << name << '*' << endl;
    return commands[getIndex(name)];
}

Task* Graph::getTask (int id) {
    return (id != -1 ? tasks[id] : nullptr);
}