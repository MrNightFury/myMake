#include "hash.h"
using namespace std;

string calculateHash (string fileName) {
    fstream file(fileName, ios::binary | ios::in);
    if (file.fail()) {
        return "a";
    }
 
    int size = 0;
    char *input = 0;
 
    file.seekg(0, ios::end);
    size = file.tellg();
    file.seekg(0, ios::beg);
 
    input = new char[(int)size];
    for (register int i = 0; !file.eof(); ++i)
        input[i] = file.get();
 
    string md5hash = md5(input);
    return md5hash;
}