// rsa.h
#ifndef RSA_H
#define RSA_H

#include <string>
#include <vector>

class RSA {
private:
    unsigned long long p, q;        // 소수
    unsigned long long n;           // n = p * q
    unsigned long long e;           // 공개 지수
    unsigned long long d;           // 개인 지수
    unsigned long long phi;         // φ(n) = (p-1)(q-1)

    bool isPrime(unsigned long long num);
    unsigned long long gcd(unsigned long long a, unsigned long long b);
    unsigned long long modInverse(unsigned long long a, unsigned long long m);
    unsigned long long modPow(unsigned long long base, unsigned long long exponent, unsigned long long modulus);
    unsigned long long generateRandomPrime(unsigned long long min, unsigned long long max);
    bool millerRabinTest(unsigned long long n, int k);
    unsigned long long modPowForPrimeTest(unsigned long long base, unsigned long long exp, unsigned long long mod);

public:
    RSA();  // 기본 생성자에서 키 생성
    RSA(unsigned long long d, unsigned long long n);  // 복호화용 생성자
    // Getter 함수들 (DB 저장용)
    unsigned long long getPublicKey() const { return e; }
    unsigned long long getPublicModulus() const { return n; }
    unsigned long long getPrivateKey() const { return d; }
    unsigned long long getPrimeP() const { return p; }
    unsigned long long getPrimeQ() const { return q; }

    // 암호화/복호화 함수
    std::vector<unsigned long long> encrypt(const std::string& message);
    std::string decrypt(const std::vector<unsigned long long>& encrypted);
};

#endif