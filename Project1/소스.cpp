#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("������ �� �� �����ϴ�: " + filePath);
    }

    std::vector<uint8_t> fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return fileContents;
}

int main() {
    try {
        std::string filePath;
        std::cout << "���� ��θ� �Է��ϼ���: ";
        std::getline(std::cin, filePath);

        std::vector<uint8_t> fileData = readBinaryFile(filePath);

        std::cout << "���� ������ ���������� �о����ϴ�. ���� �������� ũ��: " << fileData.size() << " ����Ʈ\n";
    }
    catch (const std::exception& e) {
        std::cerr << "����: " << e.what() << '\n';
    }

    return 0;
}
