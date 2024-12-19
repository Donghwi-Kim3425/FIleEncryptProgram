#include "file_io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// 파일을 바이너리 형식으로 읽어 vector<uint8_t> 반환
vector<uint8_t> readFile(const string& filePath) {
    cout << "Attempting to read file: " << filePath << endl;  // 경로 출력

    ifstream file(filePath, ios::binary);  // 바이너리 모드로 파일 열기
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for reading: " + filePath);
    }

    // 파일 크기 계산
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();  // 파일의 끝까지 이동하여 크기 구하기
    file.seekg(0, ios::beg);         // 파일 시작 지점으로 이동

    cout << "File size: " << fileSize << " bytes" << endl;  // 파일 크기 출력

    vector<uint8_t> buffer(fileSize);  // 파일 크기만큼 버퍼 할당
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);  // 파일 내용 읽기

    if (!file) {  // 읽기 실패 여부 확인
        throw runtime_error("Error while reading file: " + filePath);
    }

    file.close();  // 파일 닫기
    return buffer;  // 읽은 데이터 반환
}

// vector<uint8_t> 형식으로 데이터를 파일에 바이너리 형식으로 쓰기
void writeFile(const string& filePath, const vector<uint8_t>& data) {
    ofstream file(filePath, ios::binary);  // 바이너리 모드로 열기
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing: " + filePath);
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    // 쓰기 에러 확인
    if (!file.good()) {
        throw runtime_error("Error writing to file: " + filePath);
    }

    file.close(); // 파일 닫기
}
