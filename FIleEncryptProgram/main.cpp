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
                // 파일 읽기 (바이너리)
                vector<uint8_t> fileData = readFile(filePath);
                string aesKey = AES::generateRandomKey();

                // 다이제스트 생성 및 저장
                string digestPath = "digest." + filePath;
                sha256.update(aesKey);
                writeStringFile(digestPath, sha256.digest());

                // AES 키 암호화 및 정보 출력
                string encryptedKeyStr = rsa.encryptToString(aesKey);
                cout << "\n=== RSA Key Information ===" << endl;
                cout << "Public Key (e): " << rsa.getPublicKey() << endl;
                cout << "Public Modulus (n): " << rsa.getPublicModulus() << endl;
                cout << "Encrypted AES Key (string): " << encryptedKeyStr << endl;
                cout << "===========================\n" << endl;

                // 데이터 암호화 및 저장 (바이너리)
                AES aes(aesKey);
                vector<uint8_t> encryptedData = aes.encrypt(fileData);
                writeFile("enc." + filePath, encryptedData);

                cout << "File encrypted and saved as 'enc." << filePath << "'." << endl;
            }
            else {
                // 복호화 과정
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

                // AES 키 복호화
                string aesKey = rsa.decryptFromString(encryptedKeyStr);

                // 다이제스트 검증
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

                // 암호화된 파일 읽기 (바이너리)
                vector<uint8_t> encryptedData = readFile(filePath);

                // 복호화 및 저장 (바이너리)
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