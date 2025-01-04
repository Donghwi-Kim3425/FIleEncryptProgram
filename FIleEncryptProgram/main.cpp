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
        unsigned long long privateKey, prime1, prime2;
        RSA::readPrivateKeyFromFile("D:\\private_key.txt", privateKey, prime1, prime2);
        RSA rsa(privateKey, prime1, prime2);
        SHA256 sha256;

        while (true) {
            string filePath, operation;
            cout << "Enter the path to the file (or type 'exit' to quit): ";
            getline(cin, filePath);
            if (filePath == "exit") break;

            cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt, or 'exit' to quit): ";
            getline(cin, operation);
            if (operation == "exit") break;

            if (operation != "encrypt" && operation != "decrypt") {
                cerr << "Invalid operation. Please enter 'encrypt' or 'decrypt'." << endl;
                continue;
            }

            if (operation == "encrypt") {
                // ���� �б� (���̳ʸ�)
                vector<uint8_t> fileData = readFile(filePath);
                string aesKey = AES::generateRandomKey();

                // ��������Ʈ ���� �� ����
                string digestPath = "digest." + filePath;
                sha256.update(aesKey);
                writeStringFile(digestPath, sha256.digest());

                // AES Ű ��ȣȭ �� ���� ���
                string encryptedKeyStr = rsa.encryptToString(aesKey);
                cout << "\n=== RSA Key Information ===" << endl;
                cout << "Public Key (e): " << rsa.getPublicKey() << endl;
                cout << "Public Modulus (n): " << rsa.getPublicModulus() << endl;
                cout << "Encrypted AES Key (string): " << encryptedKeyStr << endl;
                cout << "===========================\n" << endl;

                // ������ ��ȣȭ �� ���� (���̳ʸ�)
                AES aes(aesKey);
                vector<uint8_t> encryptedData = aes.encrypt(fileData);
                writeFile("enc." + filePath, encryptedData);

                cout << "File encrypted and saved as 'enc." << filePath << "'." << endl;
            }
            else {
                // ��ȣȭ ����
                sha256 = SHA256();
                unsigned long long modulus, publicKey;
                cout << "Enter the public modulus (n): ";
                cin >> modulus;
                cout << "Enter the public key (e): ";
                cin >> publicKey;
                cin.ignore();

                cout << "Enter the encrypted AES key (string): ";
                string encryptedKeyStr;
                getline(cin, encryptedKeyStr);

                // AES Ű ��ȣȭ
                string aesKey = rsa.decryptFromString(encryptedKeyStr);

                // ��������Ʈ ����
                string originalPath = filePath;
                if (filePath.substr(0, 4) == "enc.") {
                    originalPath = filePath.substr(4);
                }
                string digestPath = "digest." + originalPath;

                sha256.update(aesKey);
                string keyDigest = sha256.digest();
                string previousDigest = readStringFile(digestPath);

                cout << "Previous digest: " << previousDigest << endl;
                cout << "Current key digest: " << keyDigest << endl;
                cout << "Decrypted AES key: " << aesKey << endl;

                if (keyDigest != previousDigest) {
                    cerr << "Key mismatch! Cannot proceed with decryption." << endl;
                    continue;
                }

                // ��ȣȭ�� ���� �б� (���̳ʸ�)
                vector<uint8_t> encryptedData = readFile(filePath);

                // ��ȣȭ �� ���� (���̳ʸ�)
                AES aes(aesKey);
                vector<uint8_t> decryptedData = aes.decrypt(encryptedData);
                writeFile("dec." + filePath.substr(4), decryptedData);

                cout << "File decrypted and saved as 'dec." << filePath.substr(4) << "'." << endl;
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