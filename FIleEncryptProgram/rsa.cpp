#include "rsa.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <ctime>

RSA::RSA(unsigned long long privateKey, unsigned long long prime1, unsigned long long prime2)
    : d(privateKey), p(prime1), q(prime2) {
    n = p * q;
    phi = (p - 1) * (q - 1);

    // private key(d)에 맞는 public key(e) 계산
    e = modInverse(d, phi);

    std::cout << "Calculated Public Key (e): " << e << std::endl;
}

bool RSA::isPrime(unsigned long long num) {
    if (num < 2) return false;
    for (unsigned long long i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

unsigned long long RSA::gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

unsigned long long RSA::modInverse(unsigned long long a, unsigned long long m) {
    unsigned long long m0 = m, t, q;
    unsigned long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    return (x1 + m0) % m0;
}

unsigned long long RSA::modPow(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
    unsigned long long result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

void RSA::generateRandomPublicKey() {
    std::srand(std::time(nullptr));
    do {
        e = 2 + (std::rand() % (phi - 2)); // 2 <= e < φ(n)
    } while (gcd(e, phi) != 1);
    std::cout << "Generated Public Key (e): " << e << std::endl;
}

std::vector<unsigned long long> RSA::encrypt(const std::string& message) {
    std::vector<unsigned long long> encrypted;
    for (char ch : message) {
        encrypted.push_back(modPow(static_cast<unsigned long long>(ch), e, n));
    }
    return encrypted;
}

std::string RSA::decrypt(const std::vector<unsigned long long>& encrypted) {
    std::string decrypted;
    for (unsigned long long enc : encrypted) {
        decrypted.push_back(static_cast<char>(modPow(enc, d, n)));
    }
    return decrypted;
}


std::string RSA::encryptToString(const std::string& message) {
    std::vector<unsigned long long> encrypted = encrypt(message);
    std::stringstream ss;
    for (size_t i = 0; i < encrypted.size(); ++i) {
        ss << encrypted[i];
        if (i < encrypted.size() - 1) {
            ss << ","; // 구분자 추가
        }
    }
    return ss.str();
}

std::string RSA::decryptFromString(const std::string& encryptedStr) {
    std::vector<unsigned long long> encrypted;
    std::stringstream ss(encryptedStr);
    std::string value;
    while (std::getline(ss, value, ',')) { // 쉼표로 구분
        encrypted.push_back(std::stoull(value));
    }
    return decrypt(encrypted);
}


void RSA::readPrivateKeyFromFile(const std::string& filePath, unsigned long long& privateKey, unsigned long long& prime1, unsigned long long& prime2) {
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("Failed to open private key file.");
    }
    file >> privateKey >> prime1 >> prime2;
}
