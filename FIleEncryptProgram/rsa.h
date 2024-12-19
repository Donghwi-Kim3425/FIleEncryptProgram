#ifndef RSA_H
#define RSA_H

#include <string>

using namespace std;

void encryptRSA(const string& data, const string& publicKey, string& encrypted);
void decryptRSA(const string& data, const string& privateKey, string& decrypted);

#endif // !RSA.H
