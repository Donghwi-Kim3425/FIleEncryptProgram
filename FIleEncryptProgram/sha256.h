#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <vector>
#include <cstdint>

class SHA256 {
private:
	void transform(const uint8_t block[64]);
	void pad();
	void process();
	uint32_t h[8];
	std::vector<uint8_t> buffer;
	uint64_t totalLength;
public:
	SHA256();
	void update(const std::string& data);
	std::string digest();
	uint32_t rotr(uint32_t value, unsigned int bits);
};

#endif // !SHA256
