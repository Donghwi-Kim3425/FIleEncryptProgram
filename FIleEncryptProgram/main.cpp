#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <mysqlx/xdevapi.h>
#include "aes.h"
#include "file_io.h"
#include "rsa.h"
#include "sha256.h"

using namespace std;

int main() {
    try {
        mysqlx::Session session("localhost", 33060, "root", "rescuepos12.");
        mysqlx::Schema db = session.getSchema("file_encryption");
        mysqlx::Table table = db.getTable("files");
        unsigned long long privateKey, prime1, prime2;
        RSA::readPrivateKeyFromFile("privateKey.txt", privateKey, prime1, prime2);
        RSA rsa(privateKey, prime1, prime2);

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

                // �ؽ� ��Ʈ ��ü ���� �� �ʱ�ȭ
                SHA256 sha256;

                // AES Ű ��ȣȭ �� ���� ���
                string encryptedKeyStr = rsa.encryptToString(aesKey);

                // ��������Ʈ ����
                sha256.update(aesKey);
                string digest = sha256.digest();

                // ������ ��ȣȭ
                AES aes(aesKey);
                vector<uint8_t> encryptedData = aes.encrypt(fileData);

                // DB�� ���� ����
                table.insert("file_path", "encrypted_aes_key", "sha256_digest", "public_key", "modulus")
                    .values(filePath, encryptedKeyStr, digest, rsa.getPublicKey(), rsa.getPublicModulus())
                    .execute();

                // ���� ���� ���� �� ��ȣȭ�� ���� ����
                filesystem::remove(filePath);
                writeFile(filePath, encryptedData);

                cout << "\n=== Encryption Information ===" << endl;
                cout << "Public Key (e): " << rsa.getPublicKey() << endl;
                cout << "Public Modulus (n): " << rsa.getPublicModulus() << endl;
                cout << "AES Key: " << aesKey << endl;
                cout << "Encrypted AES Key: " << encryptedKeyStr << endl;
                cout << "File encrypted successfully." << endl;
            }
            else {
                // DB���� ���� ���� ��ȸ
                auto result = table.select("encrypted_aes_key", "sha256_digest", "public_key", "modulus")
                    .where("file_path = :path")
                    .bind("path", filePath)
                    .execute();
                auto row = result.fetchOne();
                if (!row) {
                    cerr << "File information not found in database." << endl;
                    continue;
                }

                auto dbEncryptedKey = row[0].get<string>();
                auto storedDigest = row[1].get<string>();
                auto dbPublicKey = row[2].get<unsigned long long>();
                auto dbModulus = row[3].get<unsigned long long>();

                cout << "dbEncrypted Key: " << dbEncryptedKey << endl;
                cout << "dbDigest: " << storedDigest << endl;
                cout << "dbPublicKey: " << dbPublicKey << endl;
                cout << "dbModulus: " << dbModulus << endl;

                // �ؽ� ��Ʈ ��ü ���� �� �ʱ�ȭ
                SHA256 sha256;

                // AES Ű ��ȣȭ
                auto decryptedAesKey = rsa.decryptFromString(dbEncryptedKey);

                cout << "Decrypted AES Key: " << decryptedAesKey << endl;

                // ��������Ʈ ����
                sha256.update(decryptedAesKey);
                string keyDigest = sha256.digest();

                cout << "KeyDigest: " << keyDigest << endl;
                if (keyDigest != storedDigest) {
                    cerr << "Key verification failed! Cannot proceed with decryption." << endl;
                    continue;
                }

                // ��ȣȭ�� ���� �б� (���̳ʸ�)
                vector<uint8_t> encryptedData = readFile(filePath);

                // ��ȣȭ �� ���� (���̳ʸ�)
                AES aes(decryptedAesKey);
                vector<uint8_t> decryptedData = aes.decrypt(encryptedData);

                // ��ȣȭ�� ���� ���� �� ��ȣȭ�� ���� ����
                filesystem::remove(filePath);
                writeFile(filePath, decryptedData);

                // DB���� ���ڵ� ����
                table.remove()
                    .where("file_path = :path")
                    .bind("path", filePath)
                    .execute();

                cout << "File decrypted successfully." << endl;
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
