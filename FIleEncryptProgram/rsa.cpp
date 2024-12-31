// rsa.cpp
#include "rsa.h"
#include <random>
#include <chrono>
#include <stdexcept>
#include <sstream>

// ���� ������ ���� �õ� ����
static std::mt19937_64 generateRNG() {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return std::mt19937_64(seed);
}

static std::mt19937_64 rng = generateRNG();

unsigned long long RSA::modPowForPrimeTest(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

bool RSA::millerRabinTest(unsigned long long n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    // n-1 = d * 2^r ���·� ǥ��
    unsigned long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    std::uniform_int_distribution<unsigned long long> dist(2, n - 2);

    // k���� �׽�Ʈ �ݺ�
    for (int i = 0; i < k; i++) {
        unsigned long long a = dist(rng);
        unsigned long long x = modPowForPrimeTest(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool isProbablePrime = false;
        unsigned long long d_temp = d;
        while (d_temp != n - 1) {
            x = (x * x) % n;
            d_temp *= 2;

            if (x == 1) return false;
            if (x == n - 1) {
                isProbablePrime = true;
                break;
            }
        }

        if (!isProbablePrime)
            return false;
    }

    return true;
}

unsigned long long RSA::generateRandomPrime(unsigned long long min, unsigned long long max) {
    std::uniform_int_distribution<unsigned long long> dist(min, max);

    while (true) {
        unsigned long long candidate = dist(rng);
        // Ȧ���� �����
        if (candidate % 2 == 0)
            candidate++;

        // Miller-Rabin �׽�Ʈ�� �Ҽ� �Ǻ� (k=5�� ����)
        if (millerRabinTest(candidate, 5))
            return candidate;
    }
}

RSA::RSA() {
    // �н������� ������ ���� ���� (���� ���ÿ��� �� ū ���� �ʿ�)
    unsigned long long min_prime = 100;
    unsigned long long max_prime = 500;

    // ���� �ٸ� �� �Ҽ� ����
    p = generateRandomPrime(min_prime, max_prime);
    do {
        q = generateRandomPrime(min_prime, max_prime);
    } while (p == q);

    n = p * q;
    phi = (p - 1) * (q - 1);

    // ���� ���� e ����
    // �Ϲ������� 65537�� ���������, ���⼭�� �� ���� �� ���
    e = 17;
    while (gcd(e, phi) != 1) {
        e += 2;
    }

    // ���� ���� d ���
    d = modInverse(e, phi);
}

RSA::RSA(unsigned long long d, unsigned long long n) {
    this->d = d;
    this->n = n;
    // ��ȣȭ�� �� ���̹Ƿ� �ٸ� ������ �ʿ� ����
    this->e = 0;
    this->p = 0;
    this->q = 0;
    this->phi = 0;
}

bool RSA::isPrime(unsigned long long num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;

    for (unsigned long long i = 3; i * i <= num; i += 2) {
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
    unsigned long long m0 = m;
    unsigned long long y = 0, x = 1;

    if (m == 1) return 0;

    while (a > 1) {
        unsigned long long q = a / m;
        unsigned long long t = m;
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }

    if (x < 0) x += m0;
    return x;
}

unsigned long long RSA::modPow(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
    unsigned long long result = 1;
    base = base % modulus;

    while (exponent > 0) {
        if (exponent & 1) result = (result * base) % modulus;
        base = (base * base) % modulus;
        exponent >>= 1;
    }
    return result;
}

std::vector<unsigned long long> RSA::encrypt(const std::string& message) {
    std::vector<unsigned long long> encrypted;
    for (char c : message) {
        encrypted.push_back(modPow(static_cast<unsigned long long>(c), e, n));
    }
    return encrypted;
}

std::string RSA::decrypt(const std::vector<unsigned long long>& encrypted) {
    std::string decrypted;
    for (unsigned long long c : encrypted) {
        decrypted += static_cast<char>(modPow(c, d, n));
    }
    return decrypted;
}