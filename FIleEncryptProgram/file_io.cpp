#include "file_io.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// ���� �б� �Լ�
std::vector<uint8_t> readFile(const std::string& filePath) {
    // ������ ���̳ʸ� ���� �б�
    ifstream file(filePath, ios::binary);
    
    if (!file) {
        throw runtime_error("Failed to open file: " + filePath);
    }

    // ���� ũ�� �˾Ƴ���
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // ���� ������ vector�� ����
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    if (!file) {
        throw runtime_error("Error reading file: " + filePath);
    }

    file.close();
    return buffer;
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
