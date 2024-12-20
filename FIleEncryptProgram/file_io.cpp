#include "file_io.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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
    // ������ ���̳ʸ� ���� ����
    ofstream file(filePath, ios::binary);
    
    if (!file) {
        throw runtime_error("Failed to open file for writing: " + filePath);
    }

    // �����͸� ���Ͽ� ����
    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    if (!file) {
        throw runtime_error("Error writing to file: " + filePath);
    }

    file.close();
}
