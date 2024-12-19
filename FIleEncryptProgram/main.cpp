#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "seed.h"
#include "rsa.h"
#include "file_io.h"


using namespace std;

int main() {
    try {
        string filePath, operation;

        // ���� ��� �Է� �ޱ�
        cout << "Enter the path to the file: ";
        getline(cin, filePath);

        // ��ȣȭ �Ǵ� ��ȣȭ ����
        cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
        getline(cin, operation);

        // ���� ó��
        if (operation != "encrypt" && operation != "decrypt")
            throw invalid_argument("Invalid operation. Please enter 'encrypt' or 'decrypt'");

        // ���� Ű ����
        string key = AES::generateRandomKey();
        cout << "Generated Random Key (hex): " << key << endl;

        // AES ��ü ����
        AES aes(key);

        // ���� �б�
        string fileContent = readFile(filePath);
        try {
            fileContent = readFile(filePath);
            cout << "File content read successfully." << endl;
        }
        catch (const exception& e) {
            throw runtime_error("Error reading file: " + string(e.what()));
        }

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
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

