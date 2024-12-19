#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

using namespace std;

string readFile(const string& filePath);
void writeFile(const string& filePath, const string& data);

#endif // FILE_IO_H
