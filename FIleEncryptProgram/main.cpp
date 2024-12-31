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
                // RSA 키쌍 생성
                RSA rsa;

                // 파일 읽기
                vector<uint8_t> fileData = readFile(filePath);

                // AES 키 생성
                string aesKey = AES::generateRandomKey();
                cout << "Generated AES Key: " << aesKey << endl;

                // AES 키 다이제스트 생성
                SHA256 sha256;
                sha256.update(aesKey);
                string keyDigest = sha256.digest();
                cout << "SHA-256 Digest of AES Key: " << keyDigest << endl;

                // RSA로 AES 키 암호화
                vector<unsigned long long> encryptedKey = rsa.encrypt(aesKey);

                // 암호화에 사용된 키 정보 출력 (향후 DB 저장 대상)
                cout << "\n=== RSA Key Information (Save these for decryption) ===" << endl;
                cout << "Public Key (e): " << rsa.getPublicKey() << endl;
                cout << "Public Modulus (n): " << rsa.getPublicModulus() << endl;
                cout << "Private Key (d): " << rsa.getPrivateKey() << endl;
                cout << "Encrypted AES Key: " << toHexString(encryptedKey) << endl;
                cout << "================================================\n" << endl;

                // 파일 데이터 암호화
                AES aes(aesKey);
                vector<uint8_t> encryptedData = aes.encrypt(fileData);

                // 암호화된 데이터 저장
                writeFile("enc." + filePath, encryptedData);
            }
            else if (operation == "decrypt") {
                // RSA 키 정보 입력 받기
                cout << "Enter the private key (d): ";
                unsigned long long privateKey;
                cin >> privateKey;

                cout << "Enter the public modulus (n): ";
                unsigned long long modulus;
                cin >> modulus;

                // RSA 객체 생성 및 키 설정 (복호화용)
                RSA rsa(privateKey, modulus);
                // 여기서는 복호화만 할 것이므로 개인키와 모듈러스만 필요
                // 실제로는 이 부분을 RSA 클래스에 setDecryptionKeys 같은 함수를 만들어 처리하는 것이 좋음

                // 암호화된 AES 키 입력 받기
                cout << "Enter the encrypted AES key (hex values separated by spaces): ";
                string encryptedKeyHex;
                cin.ignore();
                getline(cin, encryptedKeyHex);

                // RSA로 AES 키 복호화 시도
                vector<unsigned long long> encryptedKey = fromHexString(encryptedKeyHex);
                string aesKey;
                try {
                    aesKey = rsa.decrypt(encryptedKey);  // 잘못된 키를 입력하면 여기서 예외 발생 가능
                }
                catch (const exception& e) {
                    cerr << "Failed to decrypt AES key. Invalid RSA keys provided." << endl;
                    continue;
                }

                // AES 키 다이제스트 생성
                SHA256 sha256;
                sha256.update(aesKey);
                string keyDigest = sha256.digest();
                cout << "SHA-256 Digest of decrypted AES Key: " << keyDigest << endl;

                // 암호화된 파일 읽기
                vector<uint8_t> encryptedData = readHexFile(filePath);

                // AES 복호화 수행
                AES aes(aesKey);
                vector<uint8_t> decryptedData;
                try {
                    decryptedData = aes.decrypt(encryptedData);
                }
                catch (const exception& e) {
                    cerr << "Failed to decrypt file. The AES key might be incorrect." << endl;
                    continue;
                }

                // 복호화된 데이터 저장
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