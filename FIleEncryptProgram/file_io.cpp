#include "file_io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// ������ ���̳ʸ� �������� �о� vector<uint8_t> ��ȯ
vector<uint8_t> readFile(const string& filePath) {
    cout << "Attempting to read file: " << filePath << endl;  // ��� ���

    ifstream file(filePath, ios::binary);  // ���̳ʸ� ���� ���� ����
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for reading: " + filePath);
    }

    // ���� ũ�� ���
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();  // ������ ������ �̵��Ͽ� ũ�� ���ϱ�
    file.seekg(0, ios::beg);         // ���� ���� �������� �̵�

    cout << "File size: " << fileSize << " bytes" << endl;  // ���� ũ�� ���

    vector<uint8_t> buffer(fileSize);  // ���� ũ�⸸ŭ ���� �Ҵ�
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);  // ���� ���� �б�

    if (!file) {  // �б� ���� ���� Ȯ��
        throw runtime_error("Error while reading file: " + filePath);
    }

    file.close();  // ���� �ݱ�
    return buffer;  // ���� ������ ��ȯ
}

// vector<uint8_t> �������� �����͸� ���Ͽ� ���̳ʸ� �������� ����
void writeFile(const string& filePath, const vector<uint8_t>& data) {
    ofstream file(filePath, ios::binary);  // ���̳ʸ� ���� ����
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing: " + filePath);
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    // ���� ���� Ȯ��
    if (!file.good()) {
        throw runtime_error("Error writing to file: " + filePath);
    }

    file.close(); // ���� �ݱ�
}
