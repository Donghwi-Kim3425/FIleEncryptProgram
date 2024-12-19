#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector> // vector<uint8_t>를 사용하기 위해 추가

using namespace std;

// 바이너리 데이터를 읽고 반환하는 함수
vector<uint8_t> readFile(const string& filePath);

// 바이너리 데이터를 파일에 쓰는 함수
void writeFile(const string& filePath, const vector<uint8_t>& data);

#endif // FILE_IO_H
