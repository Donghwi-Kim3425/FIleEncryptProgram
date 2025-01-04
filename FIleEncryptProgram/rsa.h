#ifndef RSA_H
#define RSA_H

#include <string>
#include <vector>

class RSA {
private:
    unsigned long long p, q;       // 소수
    unsigned long long n;         // n = p * q
    unsigned long long e;         // 공개 지수
    unsigned long long d;         // 개인 지수
    unsigned long long phi;       // φ(n) = (p-1)(q-1)

    bool isPrime(unsigned long long num);
    unsigned long long gcd(unsigned long long a, unsigned long long b);
    unsigned long long modInverse(unsigned long long a, unsigned long long m);
    unsigned long long modPow(unsigned long long base, unsigned long long exponent, unsigned long long modulus);

public:
    RSA(unsigned long long privateKey, unsigned long long prime1, unsigned long long prime2);
    void generateRandomPublicKey();

    // Getter 함수들
    unsigned long long getPublicKey() const { return e; }
    unsigned long long getPublicModulus() const { return n; }
    unsigned long long getPrivateKey() const { return d; }

    // 암호화/복호화 함수
    std::vector<unsigned long long> encrypt(const std::string& message);
    std::string encryptToString(const std::string& message);
    std::string decrypt(const std::vector<unsigned long long>& encrypted);
    std::string decryptFromString(const std::string& encryptedStr);

    // 파일에서 개인키 읽기
    static void readPrivateKeyFromFile(const std::string& filePath, unsigned long long& privateKey, unsigned long long& prime1, unsigned long long& prime2);
};

#endif
