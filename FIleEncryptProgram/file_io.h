#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>

// ���� ����� �Լ�
std::vector<uint8_t> readFile(const std::string& filePath);
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data);
void writeStringFile(const std::string& filePath, const std::string& data);
std::string readStringFile(const std::string& filePath);

// ��ƿ��Ƽ �Լ�
std::string bytesToHex(const std::vector<uint8_t>& bytes);
std::vector<uint8_t> hexToBytes(const std::string& hex);
std::string bytesToString(const std::vector<uint8_t>& bytes);
std::string toHexString(const std::vector<unsigned long long>& data);
std::vector<unsigned long long> fromHexString(std::string hexStr);

#endif // FILE_IO_H
