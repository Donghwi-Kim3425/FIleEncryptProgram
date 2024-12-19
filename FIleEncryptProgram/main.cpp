#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "seed.h"
#include "rsa.h"
#include "file_io.h"


using namespace std;

int main() {
    string filePath, operation;
    
    // 파일 경로 입력 받기
    cout << "Enter the path to the file: ";
    getline(cin, filePath);

    // 암호화 또는 복호화 선택
    cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
    getline(cin, operation);

    // 랜덤 키 생성
    string key = AES::generateRandomKey();
    cout << "Generated Random Key (hex): " << key << endl;

    // AES 객체 생성
    AES aes(key);

    // 파일 읽기
    string fileContent = readFile(filePath);
    cout << "File content read successfully." << endl;

    vector<uint8_t> inputData(fileContent.begin(), fileContent.end());
    vector<uint8_t> outputData;

    if (operation == "encrypt") {
        outputData = aes.encrypt(inputData);
        writeFile(filePath + ".enc.txt", string(outputData.begin(), outputData.end()));
        cout << "File encrypted and saved as " << filePath + ".enc.txt" << endl;
    }
    else if (operation == "decrypt") {
        outputData = aes.decrypt(inputData); writeFile(filePath + ".dec.txt", string(outputData.begin(), outputData.end())); 
        cout << "File decrypted and saved as " << filePath + ".dec.txt" << endl; 
    }
    else { 
        cerr << "Invalid operation. Please enter 'encrypt' or 'decrypt'." << endl; 
    }

    return 0;
}

