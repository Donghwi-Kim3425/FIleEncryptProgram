#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector> // vector<uint8_t>�� ����ϱ� ���� �߰�

using namespace std;

// ���̳ʸ� �����͸� �а� ��ȯ�ϴ� �Լ�
vector<uint8_t> readFile(const string& filePath);

// ���̳ʸ� �����͸� ���Ͽ� ���� �Լ�
void writeFile(const string& filePath, const vector<uint8_t>& data);

#endif // FILE_IO_H
