#ifndef AES_H
#define AES_H

#include <string>
#include <vector>
#include <cstdint>

using namespace std;

class AES {
public:
	AES(const string& key);
	static string generateRandomKey();
	static vector<uint8_t> hexToBytes(const string& hex); // 16진수 키 변환 함수 
	vector<uint8_t>encrypt(const vector<uint8_t>& data);
	vector<uint8_t>decrypt(const vector<uint8_t>& data);
private:
	void keyExpansion();
	void addRoundKey(int round);
	void subBytes();
	void shiftRows();
	void mixColumns();
	void invSubBytes();
	void invShiftRows();
	void invMixColumns();

	vector<vector<uint8_t>> state;
	vector<uint32_t> roundKeys;
	vector<uint8_t> key;
	static const uint8_t Sbox[256];
	static const uint8_t InvSbox[256];
	static const uint8_t Rcon[11];
	static uint8_t gmul(uint8_t a, uint8_t b);
};


#endif // AES.H
