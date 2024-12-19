#ifndef SEED_H
#define SEED_H

#include <string>

using namespace std;

void encryptSEED(const string& data, const string key, string encrypted);
void decryptSEED(const string& data, const string key, string decrypted);

#endif // !SEED.H
