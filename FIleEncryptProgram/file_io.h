#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>

// 파일 읽기 함수 선언
std::vector<uint8_t> readFile(const std::string& filePath);

// 파일 쓰기 함수 선언
void writeFile(const std::string& filePath, const std::vector<uint8_t>& data);

// 바이트 데이터를 16진수 문자열로 변환
std::string bytesToHex(const std::vector<uint8_t>& bytes);

#endif // FILE_IO_H
