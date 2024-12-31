#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>

// ���� �б� �Լ� (���̳ʸ�)
std::vector<uint8_t> readFile(const std::string& filePath);

// ���� �б� �Լ� (16����)
std::vector<uint8_t> readHexFile(const std::string& filePath);

// ���� ���� �Լ� (16����)
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data);

// ���� ���� �Լ� (string)
void writeStringFile(const std::string& filePath, const std::string& data);

// ����Ʈ �����͸� 16���� ���ڿ��� ��ȯ
std::string bytesToHex(const std::vector<uint8_t>& bytes);

// 16���� ���ڿ��� ����Ʈ �����ͷ� ��ȯ
std::vector<uint8_t> hexToBytes(const std::string& hex);

// ����Ʈ �����͸� ���ڿ��� ��ȯ
std::string bytesToString(const std::vector<uint8_t>& bytes);

std::string toHexString(const std::vector<unsigned long long>& data);

std::vector<unsigned long long> fromHexString(const std::string& hexStr);

#endif // FILE_IO_H
