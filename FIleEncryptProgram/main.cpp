#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "file_io.h"

using namespace std;

int main() {
    try {
        // ����� �Է� �ޱ�
        string filePath, operation;
        cout << "Enter the path to the file: ";
        getline(cin, filePath);
        cout << "File Path: " << filePath << endl;

        cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
        getline(cin, operation);

        // �۾� ���� ����
        if (operation != "encrypt" && operation != "decrypt") {
            throw invalid_argument("Invalid operation. Please enter 'encrypt' or 'decrypt'");
        }

        // ��ȣȭ ����
        if (operation == "encrypt") {
            // ���� �б�
            vector<uint8_t> fileData = readFile(filePath);

            // ��ȣȭ ����
            string key = AES::generateRandomKey();
            cout << "Generated Random Key (hex): " << key << endl;

            AES aes(key);
            vector<uint8_t> encryptedData = aes.encrypt(fileData);

            // ��ȣȭ�� ������ ����
            writeFile("enc." + filePath, encryptedData);
        }
        // ��ȣȭ ����
        else {
            // Ű �Է� �ޱ�
            string key;
            cout << "Enter decryption key: ";
            cin >> key;

            // ��ȣȭ�� ���� �б�
            vector<uint8_t> encryptedData = readHexFile(filePath);

            // ��ȣȭ ����
            AES aes(key);
            vector<uint8_t> decryptedData = aes.decrypt(encryptedData);

            // ��ȣȭ�� ������ ����
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