#ifndef AES_H
#define AES_H

#include <string>
#include <vector>
#include <cstdint>

class AES {
	std::vector<std::vector<uint8_t>> state;
	std::vector<uint8_t> roundKeys;
	std::vector<uint8_t> key;
	void keyExpansion();
	void addRoundKey(int round);
	void subBytes();
	void shiftRows();
	void mixColumns();
	void invSubBytes();
	void invShiftRows();
	void invMixColumns();
	static uint8_t gmul(uint8_t a, uint8_t b);
	static const uint8_t Sbox[256];
	static const uint8_t InvSbox[256];
	static const uint8_t Rcon[11];
public:
	AES(const std::string& key);
	static std::string generateRandomKey();
	static std::vector<uint8_t> padData(const std::vector<uint8_t>& data); // 패딩 추가 함수
	static std::vector<uint8_t> unpadData(const std::vector<uint8_t>& data); // 패딩 제거 함수
	std::vector<uint8_t>encrypt(const std::vector<uint8_t>& data);
	std::vector<uint8_t>decrypt(const std::vector<uint8_t>& data);
};


#endif // AES.H
