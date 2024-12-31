#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>

// 파일 읽기 함수 (바이너리)
std::vector<uint8_t> readFile(const std::string& filePath);

// 파일 읽기 함수 (16진수)
std::vector<uint8_t> readHexFile(const std::string& filePath);

// 파일 쓰기 함수 (16진수)
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data);

// 파일 쓰기 함수 (string)
void writeStringFile(const std::string& filePath, const std::string& data);

// 바이트 데이터를 16진수 문자열로 변환
std::string bytesToHex(const std::vector<uint8_t>& bytes);

// 16진수 문자열을 바이트 데이터로 변환
std::vector<uint8_t> hexToBytes(const std::string& hex);

// 바이트 데이터를 문자열로 변환
std::string bytesToString(const std::vector<uint8_t>& bytes);

std::string toHexString(const std::vector<unsigned long long>& data);

std::vector<unsigned long long> fromHexString(const std::string& hexStr);

#endif // FILE_IO_H
