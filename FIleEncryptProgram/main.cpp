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

        vector<uint8_t> fileData;

        // ��ȣȭ �Ǵ� ��ȣȭ ����
        cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
        getline(cin, operation);

        // ���� ó��
        if (operation != "encrypt" && operation != "decrypt") {
            throw invalid_argument("Invalid operation. Please enter 'encrypt' or 'decrypt'");
        }

        // ��ȣȭ �Ǵ� ��ȣȭ ����
        vector<uint8_t> outputData;
        if (operation == "encrypt") {
            // ���� �б�
            try {
                fileData = readFile(filePath);
                cout << "File content read successfully." << endl;
                cout << "File size: " << fileData.size() << " bytes" << endl;
                cout << "File data (hex): ";
                for (size_t i = 0; i < fileData.size(); ++i) {
                    printf("%02x ", fileData[i]);
                    if (i % 16 == 15) cout << endl; // 16����Ʈ�� ���
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
                // ���� Ű ����
                string key = AES::generateRandomKey();
                cout << "Generated Random Key (hex): " << key << endl;

                // AES ��ü ����
                AES aes(key);
                outputData = aes.encrypt(fileData);  // AES ��ȣȭ
                cout << "Encryption successful. Encrypted data size: " << outputData.size() << " bytes" << endl;
            }
            catch (const exception& e) {
                cerr << "Error during encryption: " << e.what() << endl;
                return 1;  // ����
            }

            try {
                writeFile("enc." + filePath, outputData);  // ���� ����
                cout << "File encrypted and saved as " << "enc." + filePath << endl;
            }
            catch (const exception& e) {
                cerr << "Error during file writing: " << e.what() << endl;
                return 1;  // ����
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
                outputData = aes.decrypt(encryptedData);  // AES ��ȣȭ
                cout << "Decryption successful. Decrypted data size: " << outputData.size() << " bytes" << endl;
            }
            catch (const exception& e) {
                cerr << "Error during decryption: " << e.what() << endl;
                return 1;
            }

            try {
                writeStringFile("dec." + filePath, bytesToString(outputData)); // ��������
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