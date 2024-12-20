#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("파일을 열 수 없습니다: " + filePath);
    }

    std::vector<uint8_t> fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return fileContents;
}

int main() {
    try {
        std::string filePath;
        std::cout << "파일 경로를 입력하세요: ";
        std::getline(std::cin, filePath);

        std::vector<uint8_t> fileData = readBinaryFile(filePath);

        std::cout << "파일 내용을 성공적으로 읽었습니다. 읽은 데이터의 크기: " << fileData.size() << " 바이트\n";
    }
    catch (const std::exception& e) {
        std::cerr << "오류: " << e.what() << '\n';
    }

    return 0;
}
