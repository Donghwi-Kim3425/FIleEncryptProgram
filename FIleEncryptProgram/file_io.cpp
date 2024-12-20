#include "file_io.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// 파일 읽기 함수
vector<uint8_t> readFile(const string& filePath) {
    // 파일을 바이너리 모드로 읽기
    ifstream file(filePath, ios::binary);
    
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filePath);
    }
    
    // 파일 내용을 vector에 저장
    vector<uint8_t> fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    file.close();
    return fileContents;
}

// 파일 쓰기 함수
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    // 파일을 바이너리 모드로 열기
    ofstream file(filePath, ios::binary);
    
    if (!file) {
        throw runtime_error("Failed to open file for writing: " + filePath);
    }

    // 데이터를 파일에 쓰기
    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    if (!file) {
        throw runtime_error("Error writing to file: " + filePath);
    }

    file.close();
}
