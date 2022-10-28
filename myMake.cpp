#include "Makefile.h"
#include "hash.h"
#include <fstream>
#include <iostream>
using namespace std;

int main (int argc, char* argv[]) {
    Makefile mf;
    string fileName = "makefile";
    string task = "_first_task_signal_";
    for (int i = 1; i < argc; i++) {
        string arg = (string)argv[i];
        if (arg == "-f") {
            if (i == argc - 1) {
                cout << "File not specified";
                return 0;
            }
            i++;
            fileName = argv[i];
            continue;
        }
        if (arg == "-l") {
            mf.addFlag (arg);
            continue;
        }
        task = argv[i];
    }
    mf.loadFile (fileName);
    mf.log ("Task: \"" + task + '\"');
    mf.loadHash();
    mf.make(task);
    mf.saveHash();
    return 0;
}