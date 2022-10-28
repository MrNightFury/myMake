#include "Makefile.h"
#include "hash.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
using namespace std;

inline bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

string trim (string& str) {
    while (str[0] == ' ' || str[0] == '\t') {
        str.erase (0, 1);
    }
    return str;
}

Makefile::Makefile () {

}

void Makefile::loadFile (string fileName) {
    //cout << fileName << endl;
    if (logNeeded)
        logStream.open("log.txt");
    log ("Log begin");
    //cout << fileName << endl;
    log ("File: \"" + fileName + '\"');
    graph = Graph();
    ifstream fin;
    fin.open (fileName);
    if (!fin.is_open()) {
        log ("File not found", ERROR);
        cout << "File not found";
        error = true;
        return;
    }
    log ("File opened");
    
    while (!fin.eof()) {
        string line;
        getline (fin, line);
        if (line.empty()) {
            continue;
        }
        stringstream str = stringstream(line);
        string task;
        getline (str, task, ':');
        if (!graph.isExist (task)) {
            graph.addTask (task);
        }
        while (!str.eof() && str.peek() != -1) {
            string subtask;
            str >> subtask;
            if (task == ".PHONY") {
                PHONY.push_back (subtask);
                log ("Added PHONY named \"" + subtask + '\"');
            } else if (!trim(subtask).empty()) {
                graph.addLine (task, subtask);
            }
        }
        char c = fin.peek();
        while ((c == '\t' || c == ' ') && !fin.eof()) {
            string commandLine;
            getline (fin, commandLine);
            trim (commandLine);
            graph.addCommand (task, commandLine);
            c = fin.peek();
        }
    }
    log ("File loaded");
}

void Makefile::show() {
    graph.show();
}

/*
void Makefile::buildAll() {
    Graph g = Graph(graph);
    //cout << '|' << g.getIndex ("shirt");
    vector<Task> tasks;
    vector<string> commands;
    while (!g.isEmpty()) {
        tasks = g.getReadyTasks();
        for (int i = 0; i < tasks.size(); i++) {
            commands = tasks[i].commands;
            for (int j = 0; j < commands.size(); j++) {
                cout << '$' << commands[j] << endl;
            }
        }
    }
}*/

// void Makefile::build(string name, bool silent) {
//     //cout << "Begin to " << name << endl;
//     bool flag = false;
//     Task* task = graph.getTask(graph.getIndex (name));
//     for (int i = 0; i < task->from.size(); i++) {
//         bool check = calculateHash (graph.getTask(task->from[i])->name) != task->lastHashes[i];
//         //cout << "Try to " << graph.getTask(task->from[i])->name << " from " << task->name << '[' << check <<"]... ";
        
//         if (check || task->lastHashes[i] == "a") {
//             //cout << "start\n";
//             build (graph.getTask (task->from[i])->name, true);
//             task->lastHashes[i] = calculateHash (graph.getTask(task->from[i])->name);
//             flag = true;
//             //cout << '[' << task->lastHashes[i] << "]";
//         } else {
//             //cout << graph.getTask(task->from[i])->name << " is already built\n";
//         }
        
//         //cout << "**" << task. .lastHashes[i] << '\n';
//     }
//     if (flag) {
//         for (int i = 0; i < task->commands.size(); i++) {
//             system(task->commands[i].c_str());
//         }
//     } else if (!silent) {
//         cout << task->name << " is up to date\n";
//     }
// }

void Makefile::build(string name, bool silent) {
    log ("Getting \"" + name + '\"');
    //cout << "Begin to " << name << endl;
    bool flag = false;
    Task* task = graph.getTask(graph.getIndex (name));
    if (task == nullptr) {
        log ("Task not found", ERROR);
        return;
    }
    log ("Begin to \"" + task->name + '\"');

    for (int i = 0; i < task->from.size(); i++) {
        build (graph.getTask (task->from[i])->name, true);
        bool check = calculateHash (graph.getTask(task->from[i])->name) != task->lastHashes[i];


        //cout << "Try to " << graph.getTask(task->from[i])->name << " from " << task->name << '[' << check <<"]... ";
        
        if (check /*|| task->lastHashes[i] == "a"*/) {
            //cout << "start\n";
            task->lastHashes[i] = calculateHash (graph.getTask(task->from[i])->name);
            flag = true;
            //cout << '[' << task->lastHashes[i] << "]";
        } else {
            //cout << graph.getTask(task->from[i])->name << " is already built\n";
        }
        
        //cout << "**" << task. .lastHashes[i] << '\n';
    }
    if (isPhony (task->name) || !exists(task->name) || flag) {
        log ("Executing \"" + name + '\"' + (isPhony (task->name) ? "phony" : ""));
        //cout << '*' << flag << '*' << !exists(task->name) << "*\n";
        //cout << '*' << task->name << '*' << task->commands.size() << "*\n";
        for (int i = 0; i < task->commands.size(); i++) {
            //cout << task->commands[i] << '\n';
            system(task->commands[i].c_str());
        }
    } else  {
        log ('\"' + name + "\" is up to date");
        if (!silent) {
            cout << task->name << " is up to date\n";
        }
    }
}

void Makefile::loadHash() {
    log ("Hash loading...");
    ifstream fin;
    fin.open ("db.txt");
    string taskName, subtaskName, hash;
    Task* task;
    while (!fin.eof() && !fin.fail()) {
        if (fin.peek() != ':') {
            getline (fin, taskName);
            task = graph.getTask (graph.getIndex (taskName));
            if (task == nullptr) {
                log ("Not existing task tried to be loaded from database", WARN);
            }
        } else {
            fin.get();
            fin >> subtaskName >> hash;
            fin.get();
            int subtaskI = graph.getIndex (subtaskName);
            if (task == nullptr || subtaskI == -1) {
                log ("Not existing subtask tried to be loaded from database", WARN);
                continue;
            }
            for (int i = 0; i < task->from.size(); i++) {
                if (task->from[i] == subtaskI) {
                    task->lastHashes[i] = hash;
                    break;
                }
            }
        }
    }
    log ("Hash loaded");
}

void Makefile::saveHash () {
    log ("Hash saving...");
    ofstream fout;
    fout.open ("db.txt");
    int count = graph.getTaskCount();
    Task* task,* subtask;
    // for (int i = 0; i < count; i++) {
    //     task = graph.getTask(i);
    //     for (int j = 0; j < task->lastHashes.size(); j++) {
    //         cout << task->lastHashes[j];
    //     }
    // }
    for (int i = 0; i < count; i++) {
        task = graph.getTask (i);
        fout << task->name << '\n';
        for (int j = 0; j < task->from.size(); j++) {
            subtask = graph.getTask (task->from[j]);
            //cout << j << '|' << subtask->lastHashes.size() << '|' << task->lastHashes[j] << '\n';
            fout << ':' << subtask->name << ' ' << task->lastHashes[j] << '\n';
        }
    }
    log ("Hash saved");
}

void Makefile::log (string message, LogType logtype) {
    if (logNeeded)
        logStream << '[' << (logtype == LOG ? "LOG" : (logtype == WARN ? "WARN" : "ERROR")) << "] " << message << '\n';
}

// void Makefile::updateHash (string name) {
//     hashes[name] = calculateHash (name);
// }

void Makefile::showHash () {
    int count = graph.getTaskCount();
    Task* task,* subtask;
    for (int i = 0; i < count; i++) {
        task = graph.getTask(i);
    }
    for (int i = 0; i < count; i++) {
        task = graph.getTask (i);
        cout << task->name << '\n';
        for (int j = 0; j < task->from.size(); j++) {
            subtask = graph.getTask (task->from[j]);
            //cout << j << '|' << subtask->lastHashes.size() << '|' << task->lastHashes[j] << '\n';
            cout << ':' << subtask->name << ' ' << task->lastHashes[j] << '\n';
        }
    }
}

void Makefile::make (string task) {
    build (task);
}

bool Makefile::isPhony (string name) {
    for (int i = 0; i < PHONY.size(); i++) {
        if (PHONY[i] == name) {
            return true;
        }
    }
    return false;
}

void Makefile::addFlag (string flag) {
    switch (flag[1]) {
    case 'l':
        logNeeded = true;
        break;
    default:
        break;
    }
}