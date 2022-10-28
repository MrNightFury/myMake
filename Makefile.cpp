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
    if (logNeeded)
        logStream.open("log.txt");
    log ("Log begin");
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

void Makefile::build(string name, bool silent) {
    log ("Getting \"" + name + '\"');
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
        log (to_string(check) + " " + name + "[" + task->lastHashes[i] + "|" + calculateHash (graph.getTask(task->from[i])->name) + "]");
        if (check /*|| task->lastHashes[i] == "a"*/) {
            task->lastHashes[i] = calculateHash (graph.getTask(task->from[i])->name);
            flag = true;
        } else {

        }
    }
    if (isPhony (task->name) || !exists(task->name) || flag) {
        log (to_string(isPhony (task->name)) + "|" + to_string(!exists(task->name)) + "|" + to_string (flag));
        log ("Executing \"" + name + '\"' + (isPhony (task->name) ? "phony" : ""));
        for (int i = 0; i < task->commands.size(); i++) {
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
    for (int i = 0; i < count; i++) {
        task = graph.getTask (i);
        fout << task->name << '\n';
        for (int j = 0; j < task->from.size(); j++) {
            subtask = graph.getTask (task->from[j]);
            fout << ':' << subtask->name << ' ' << task->lastHashes[j] << '\n';
        }
    }
    log ("Hash saved");
}

void Makefile::log (string message, LogType logtype) {
    if (logNeeded)
        logStream << '[' << (logtype == LOG ? "LOG" : (logtype == WARN ? "WARN" : "ERROR")) << "] " << message << '\n';
}

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