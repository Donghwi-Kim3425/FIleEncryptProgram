#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "file_io.h"

using namespace std;

int main() {
    try {
        string filePath, operation;

        // 파일 경로 입력 받기
        cout << "Enter the path to the file: ";
        getline(cin, filePath);
        cout << "File Path: " << filePath << endl;

        // 암호화 또는 복호화 선택
        cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
        getline(cin, operation);

        // 에러 처리
        if (operation != "encrypt" && operation != "decrypt")
            throw invalid_argument("Invalid operation. Please enter 'encrypt' or 'decrypt'");

        // 랜덤 키 생성
        string key = AES::generateRandomKey();
        cout << "Generated Random Key (hex): " << key << endl;

        // AES 객체 생성
        AES aes(key);

        // 파일 읽기
        vector<uint8_t> fileContent;
        try {
            fileContent = readFile(filePath);
            cout << "File content read successfully." << endl;
            cout << "File size: " << fileContent.size() << " bytes" << endl;
        }
        catch (const exception& e) {
            cerr << "Error reading file: " << e.what() << endl;
            return 1;
        }

        // 암호화 또는 복호화 수행
        vector<uint8_t> outputData;
        if (operation == "encrypt") {
            outputData = aes.encrypt(fileContent);  // AES 암호화
            writeFile(filePath + ".enc", outputData);  // 파일을 바이너리로 저장
            cout << "File encrypted and saved as " << filePath + ".enc" << endl;
        }
        else if (operation == "decrypt") {
            outputData = aes.decrypt(fileContent);  // AES 복호화
            writeFile(filePath + ".dec", outputData);  // 파일을 바이너리로 저장
            cout << "File decrypted and saved as " << filePath + ".dec" << endl;
        }

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
