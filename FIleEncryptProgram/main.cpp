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
    
    // ���� ��� �Է� �ޱ�
    cout << "Enter the path to the file: ";
    getline(cin, filePath);

    // ��ȣȭ �Ǵ� ��ȣȭ ����
    cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
    getline(cin, operation);

    // ���� Ű ����
    string key = AES::generateRandomKey();
    cout << "Generated Random Key (hex): " << key << endl;

    // AES ��ü ����
    AES aes(key);

    // ���� �б�
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

