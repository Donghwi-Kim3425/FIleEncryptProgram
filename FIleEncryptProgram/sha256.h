#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <vector>
#include <cstdint>

class SHA256 {
private:
    uint32_t h[8];
    std::vector<uint8_t> buffer;
    uint64_t totalLength;
    static const uint32_t k[64];

    uint32_t rotr(uint32_t value, unsigned int bits);
    void transform(const uint8_t block[64]);
    void pad();
    void process();
    void reset();

public:
    SHA256();
    void update(const std::string& data);
    std::string digest();
};

#endif // SHA256_H
