#include "file_io.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

// ����Ʈ �����͸� 16���� ���ڿ��� ��ȯ
string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;
    ss << hex << uppercase;
    for (uint8_t byte : bytes)
        ss << setw(2) << setfill('0') << static_cast<int>(byte);
    return ss.str();
}

// ���� �б� �Լ�
vector<uint8_t> readFile(const string& filePath) {
    // ������ ���̳ʸ� ���� �б�
    ifstream file(filePath, ios::binary);
    
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filePath);
    }
    
    // ���� ������ vector�� ����
    vector<uint8_t> fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    file.close();
    return fileContents;
}

// ���� ���� �Լ�
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    // ������ �ؽ�Ʈ ���� ����
    ofstream file(filePath);
    string hexString = bytesToHex(data);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    file << hexString; // 16���� ���ڿ��� ���Ͽ� ��

    if (file.fail()) { 
        throw std::runtime_error("Error writing to file: " + filePath);
    }

    file.close();
}
