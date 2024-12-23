#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "file_io.h"

using namespace std;

int main() {
    try {
        // 사용자 입력 받기
        string filePath, operation;
        cout << "Enter the path to the file: ";
        getline(cin, filePath);
        cout << "File Path: " << filePath << endl;

        cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
        getline(cin, operation);

        // 작업 유형 검증
        if (operation != "encrypt" && operation != "decrypt") {
            throw invalid_argument("Invalid operation. Please enter 'encrypt' or 'decrypt'");
        }

        // 암호화 수행
        if (operation == "encrypt") {
            // 파일 읽기
            vector<uint8_t> fileData = readFile(filePath);

            // 암호화 수행
            string key = AES::generateRandomKey();
            cout << "Generated Random Key (hex): " << key << endl;

            AES aes(key);
            vector<uint8_t> encryptedData = aes.encrypt(fileData);

            // 암호화된 데이터 저장
            writeFile("enc." + filePath, encryptedData);
        }
        // 복호화 수행
        else {
            // 키 입력 받기
            string key;
            cout << "Enter decryption key: ";
            cin >> key;

            // 암호화된 파일 읽기
            vector<uint8_t> encryptedData = readHexFile(filePath);

            // 복호화 수행
            AES aes(key);
            vector<uint8_t> decryptedData = aes.decrypt(encryptedData);

            // 복호화된 데이터 저장
            writeStringFile("dec." + filePath, bytesToString(decryptedData));
        }

        cout << "Operation completed successfully!" << endl;
        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}