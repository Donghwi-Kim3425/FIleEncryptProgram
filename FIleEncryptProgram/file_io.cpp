#include "file_io.h"
#include <fstream>
#include <sstream>

using namespace std;

string readFile(const string& filePath) {
    ifstream file(filePath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const string& filePath, const string& data) {
    ofstream file(filePath);
    file << data;
}