#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "aes.h"
#include "file_io.h"
#include "rsa.h"
#include "sha256.h"

using namespace std;


int main() {
    try {
        while (true) {
            string filePath, operation;
            cout << "Enter the path to the file (or type 'exit' to quit): ";
            getline(cin, filePath);
            if (filePath == "exit") {
                cout << "Exiting program." << endl;
                break;
            }

            cout << "File Path: " << filePath << endl;
            cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt, or 'exit' to quit): ";
            getline(cin, operation);

            if (operation == "exit") {
                cout << "Exiting program." << endl;
                break;
            }

            if (operation != "encrypt" && operation != "decrypt") {
                cerr << "Invalid operation. Please enter 'encrypt' or 'decrypt'." << endl;
                continue;
            }

            if (operation == "encrypt") {
                // RSA Ű�� ����
                RSA rsa;

                // ���� �б�
                vector<uint8_t> fileData = readFile(filePath);

                // AES Ű ����
                string aesKey = AES::generateRandomKey();
                cout << "Generated AES Key: " << aesKey << endl;

                // AES Ű ��������Ʈ ����
                SHA256 sha256;
                sha256.update(aesKey);
                string keyDigest = sha256.digest();
                cout << "SHA-256 Digest of AES Key: " << keyDigest << endl;

                // RSA�� AES Ű ��ȣȭ
                vector<unsigned long long> encryptedKey = rsa.encrypt(aesKey);

                // ��ȣȭ�� ���� Ű ���� ��� (���� DB ���� ���)
                cout << "\n=== RSA Key Information (Save these for decryption) ===" << endl;
                cout << "Public Key (e): " << rsa.getPublicKey() << endl;
                cout << "Public Modulus (n): " << rsa.getPublicModulus() << endl;
                cout << "Private Key (d): " << rsa.getPrivateKey() << endl;
                cout << "Encrypted AES Key: " << toHexString(encryptedKey) << endl;
                cout << "================================================\n" << endl;

                // ���� ������ ��ȣȭ
                AES aes(aesKey);
                vector<uint8_t> encryptedData = aes.encrypt(fileData);

                // ��ȣȭ�� ������ ����
                writeFile("enc." + filePath, encryptedData);
            }
            else if (operation == "decrypt") {
                // RSA Ű ���� �Է� �ޱ�
                cout << "Enter the private key (d): ";
                unsigned long long privateKey;
                cin >> privateKey;

                cout << "Enter the public modulus (n): ";
                unsigned long long modulus;
                cin >> modulus;

                // RSA ��ü ���� �� Ű ���� (��ȣȭ��)
                RSA rsa(privateKey, modulus);
                // ���⼭�� ��ȣȭ�� �� ���̹Ƿ� ����Ű�� ��ⷯ���� �ʿ�
                // �����δ� �� �κ��� RSA Ŭ������ setDecryptionKeys ���� �Լ��� ����� ó���ϴ� ���� ����

                // ��ȣȭ�� AES Ű �Է� �ޱ�
                cout << "Enter the encrypted AES key (hex values separated by spaces): ";
                string encryptedKeyHex;
                cin.ignore();
                getline(cin, encryptedKeyHex);

                // RSA�� AES Ű ��ȣȭ �õ�
                vector<unsigned long long> encryptedKey = fromHexString(encryptedKeyHex);
                string aesKey;
                try {
                    aesKey = rsa.decrypt(encryptedKey);  // �߸��� Ű�� �Է��ϸ� ���⼭ ���� �߻� ����
                }
                catch (const exception& e) {
                    cerr << "Failed to decrypt AES key. Invalid RSA keys provided." << endl;
                    continue;
                }

                // AES Ű ��������Ʈ ����
                SHA256 sha256;
                sha256.update(aesKey);
                string keyDigest = sha256.digest();
                cout << "SHA-256 Digest of decrypted AES Key: " << keyDigest << endl;

                // ��ȣȭ�� ���� �б�
                vector<uint8_t> encryptedData = readHexFile(filePath);

                // AES ��ȣȭ ����
                AES aes(aesKey);
                vector<uint8_t> decryptedData;
                try {
                    decryptedData = aes.decrypt(encryptedData);
                }
                catch (const exception& e) {
                    cerr << "Failed to decrypt file. The AES key might be incorrect." << endl;
                    continue;
                }

                // ��ȣȭ�� ������ ����
                writeStringFile("dec." + filePath, bytesToString(decryptedData));
            }

            cout << "Operation completed successfully!" << endl;
        }
        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}