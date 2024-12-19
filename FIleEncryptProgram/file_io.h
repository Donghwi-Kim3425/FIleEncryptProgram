#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>

// ���� �б� �Լ� ����
std::vector<uint8_t> readFile(const std::string& filePath);

// ���� ���� �Լ� ����
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data);

#endif // FILE_IO_H
