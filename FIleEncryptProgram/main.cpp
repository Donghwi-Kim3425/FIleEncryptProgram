#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "file_io.h"

using namespace std;

int main() {
    try {
        string filePath, operation;

        // ���� ��� �Է� �ޱ�
        cout << "Enter the path to the file: ";
        getline(cin, filePath);
        cout << "File Path: " << filePath << endl;

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

        // ��ȣȭ �Ǵ� ��ȣȭ ����
        vector<uint8_t> outputData;
        if (operation == "encrypt") {
            outputData = aes.encrypt(fileContent);  // AES ��ȣȭ
            writeFile(filePath + ".enc", outputData);  // ������ ���̳ʸ��� ����
            cout << "File encrypted and saved as " << filePath + ".enc" << endl;
        }
        else if (operation == "decrypt") {
            outputData = aes.decrypt(fileContent);  // AES ��ȣȭ
            writeFile(filePath + ".dec", outputData);  // ������ ���̳ʸ��� ����
            cout << "File decrypted and saved as " << filePath + ".dec" << endl;
        }

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
