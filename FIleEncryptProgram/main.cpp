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

        vector<uint8_t> fileData;

        // 암호화 또는 복호화 선택
        cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
        getline(cin, operation);

        // 에러 처리
        if (operation != "encrypt" && operation != "decrypt") {
            throw invalid_argument("Invalid operation. Please enter 'encrypt' or 'decrypt'");
        }

        // 암호화 또는 복호화 수행
        vector<uint8_t> outputData;
        if (operation == "encrypt") {
            // 파일 읽기
            try {
                fileData = readFile(filePath);
                cout << "File content read successfully." << endl;
                cout << "File size: " << fileData.size() << " bytes" << endl;
                cout << "File data (hex): ";
                for (size_t i = 0; i < fileData.size(); ++i) {
                    printf("%02x ", fileData[i]);
                    if (i % 16 == 15) cout << endl; // 16바이트씩 출력
                }
                cout << endl;

            }
            catch (const exception& e) {
                cerr << "Error reading file: " << e.what() << endl;
                return 1;
            }

            cout << "Starting encryption..." << endl;
            cout << "Original data size: " << fileData.size() << " bytes" << endl;

            try {
                // 랜덤 키 생성
                string key = AES::generateRandomKey();
                cout << "Generated Random Key (hex): " << key << endl;

                // AES 객체 생성
                AES aes(key);
                outputData = aes.encrypt(fileData);  // AES 암호화
                cout << "Encryption successful. Encrypted data size: " << outputData.size() << " bytes" << endl;
            }
            catch (const exception& e) {
                cerr << "Error during encryption: " << e.what() << endl;
                return 1;  // 종료
            }

            try {
                writeFile("enc." + filePath, outputData);  // 파일 저장
                cout << "File encrypted and saved as " << "enc." + filePath << endl;
            }
            catch (const exception& e) {
                cerr << "Error during file writing: " << e.what() << endl;
                return 1;  // 종료
            }
        }

        else if (operation == "decrypt") {
            string key;

            cout << "Enter decryption key: ";
            cin >> key;

            cout << "Stating decrypotin..." << endl;
            cout << "Data size: " << fileData.size() << " bytes" << endl;

            try {
                AES aes(key);
                vector<uint8_t> encryptedData = readHexFile(filePath);
                outputData = aes.decrypt(encryptedData);  // AES 복호화
                cout << "Decryption successful. Decrypted data size: " << outputData.size() << " bytes" << endl;
            }
            catch (const exception& e) {
                cerr << "Error during decryption: " << e.what() << endl;
                return 1;
            }

            try {
                writeStringFile("dec." + filePath, bytesToString(outputData)); // 파일저장
                cout << "File decrypted and saved as " << "dec." + filePath << endl;
            }
            catch (const exception& e) {
                cerr << "Error during file writing: " << e.what() << endl;
                return 1;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}