#include "file_io.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

// 바이트를 16진수 문자열로 변환
string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;
    ss << hex << uppercase;
    for (uint8_t byte : bytes)
        ss << setw(2) << setfill('0') << static_cast<int>(byte);
    return ss.str();
}

// 16진수 값들을 바이트로 변환
vector<uint8_t> hexToBytes(const string& hex) {
    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

// 바이트 데이터를 문자열로 변환
string bytesToString(const vector<uint8_t>& bytes) {
    return string(bytes.begin(), bytes.end());
}

// 파일 읽기 함수 (바이너리 모드)
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

// 파일 읽기 함수 (16진수)
vector<uint8_t> readHexFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filePath);
    }
    string hexString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return hexToBytes(hexString);
}

// 파일 쓰기 함수 (16진수 값)
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    // 파일을 텍스트 모드로 열기
    ofstream file(filePath);
    string hexString = bytesToHex(data);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    file << hexString; // 16진수 문자열을 파일에 씀

    if (file.fail()) { 
        throw std::runtime_error("Error writing to file: " + filePath);
    }

    file.close();
}

// 파일 쓰기 함수 (string)
void writeStringFile(const string& filePath, const string& data) {
    ofstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing: " + filePath);
    }

    file << data;

    if (file.fail()) {
        throw runtime_error("Error writing to file: " + filePath);
    }

    file.close();
}