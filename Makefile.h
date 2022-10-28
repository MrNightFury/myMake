#ifndef __MAKEFILE_H
#define __MAKEFILE_H
#include "Graph.h"
#include <fstream>
#include <iostream>
using namespace std;

class Makefile {
    ofstream logStream;
    Graph graph;
    vector<string> PHONY;
    bool logNeeded = false;
    bool error = false;

    public: 
        Makefile ();
        void show();
        //void buildAll();
        bool isPhony (string name);
        void build (string name, bool silent = 0);
        void make (string name);

        void loadFile (string fileName);
        void loadHash ();
        void saveHash ();

        enum LogType {LOG, WARN, ERROR};
        void log (string message, LogType logType = LOG);
        bool good(){ return !error;};
        void addFlag (string flag);
        void showHash();
};

#endif