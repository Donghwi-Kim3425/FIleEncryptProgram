#include "aes.h"
#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;
AES::AES(const string& key) {
	this->key = vector<uint8_t>(key.begin(), key.end());
	keyExpansion();
}

void AES::keyExpansion() {
	// 키 확장 AES-128(16바이트) 기준
	const int keySize = 16; // 128비트
	const int expandedKeySize = 176; // 11라운드 * 16바이트
	roundKeys.clear();
	roundKeys.resize(expandedKeySize / 4); // 라운드 키를 4바이트씩 저장

	// 초기 키를 확장된 키 배열의 첫 부분에 복사
	for (int i = 0; i < keySize; ++i)
		roundKeys[i] = key[i];

	// 키 확장
	int bytesGenerated = keySize;
	int rconIndex = 1;
	uint8_t temp[4]; // 임시 워드

	while (bytesGenerated < expandedKeySize) {
		// 마지막 4바이트를 임시 버퍼에 복사
		for (int i = 0; i < 4; ++i)
			temp[i] = roundKeys[(bytesGenerated - 4) + i];

		// 16바이트 블록의 첫 번째 워드를 위한 작업
		if (bytesGenerated % keySize == 0) {
			// 1바이트씩 왼쪽으로 회전 (Rot Word)
			uint8_t k = temp[0];
			temp[0] = temp[1];
			temp[1] = temp[2];
			temp[2] = temp[3];
			temp[3] = k;

			// S-Box 치환 (Sub Word)
			for (int i = 0; i < 4; ++i)
				temp[i] = Sbox[temp[i]];

			// Rcon 추가
			temp[0] ^= Rcon[rconIndex];
			rconIndex++;

			// 확장된 키 배열에 temp 추가
			for (int i = 0; i < 4; ++i) {
				roundKeys[bytesGenerated] = roundKeys[bytesGenerated - keySize] ^ temp[i];
				bytesGenerated++;
			}
		}
	
	}
}

void AES::addRoundKey(int round) {
	// 라운드 키 추가
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			state[i][j] ^= roundKeys[round * 4 + i] >> (8 * (3 - j)) & 0xFF;
		}
	}
}

void AES::subBytes() {
	// SubBytes 단계
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j)
			state[i][j] = Sbox[state[i][j]];
	}
}

void AES::shiftRows() {
	// ShiftRows 단계
	uint8_t temp;

	// 2nd row (왼쪽 shift 1)
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;

	// 3rd row (왼쪽 shift 2)
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	// 4th row (왼쪽 shift 3)
	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;
}

void AES::mixColumns() {
	// MixColumns 단계
	for (int i = 0; i < 4; ++i) {
		uint8_t a = state[0][i];
		uint8_t b = state[1][i];
		uint8_t c = state[2][i];
		uint8_t d = state[3][i];

		state[0][i] = gmul(a, 2) ^ gmul(b, 3) ^ c ^ d;
		state[1][i] = a ^ gmul(b, 2) ^ gmul(c, 3) ^ d;
		state[2][i] = a ^ b ^ gmul(c, 2) ^ gmul(d, 3);
		state[3][i] = gmul(a, 3) ^ b ^ c ^ gmul(d, 2);
	}
}

void AES::invSubBytes() {
	// 복호화 SubBytes 단계
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j)
			state[i][j] = InvSbox[state[i][j]];
	}
}

void AES::invShiftRows() {
	// 복호화 ShiftRows 단계
	uint8_t temp;

	// 2nd row (오른쪽 shift 1)
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;

	// 3rd row (오른쪽 shift 2)
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	// 4th row (왼쪽 shift 3)
	temp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = temp;

}

void AES::invMixColumns() {
	// 복호화 MixColumns 단계
	for (int i = 0; i < 4; ++i) {
		uint8_t a = state[0][i];
		uint8_t b = state[1][i];
		uint8_t c = state[2][i];
		uint8_t d = state[3][i];

		state[0][i] = gmul(0x0E, a) ^ gmul(0x0B, b) ^ gmul(0x0D, c) ^ gmul(0x09, d);
		state[1][i] = gmul(0x09, a) ^ gmul(0x0E, b) ^ gmul(0x0B, c) ^ gmul(0x0D, d);
		state[2][i] = gmul(0x0D, a) ^ gmul(0x09, b) ^ gmul(0x0E, c) ^ gmul(0x0B, d);
		state[3][i] = gmul(0x0B, a) ^ gmul(0x0D, b) ^ gmul(0x09, c) ^ gmul(0x0E, d);
	}
}

vector<uint8_t> AES::encrypt(const vector<uint8_t>& data) {
	// 초기 상태 설정
	state = vector<vector<uint8_t>>(4, vector<uint8_t>(4));
	for (int i = 0; i < data.size(); ++i)
		state[i / 4][i % 4] = data[i];
	
	// 암호화
	addRoundKey(0);
	for (int round = 1; round < 10; ++round) {
		subBytes();
		shiftRows();
		mixColumns();
		addRoundKey(round);
	}
	subBytes();
	shiftRows();
	addRoundKey(10);

	// 결과 리턴
	vector<uint8_t> encryptedData(16);
	for (int i = 0; i < 16; ++i)
		encryptedData[i] = state[i / 4][i % 4];
	return encryptedData;
}

vector<uint8_t> AES::decrypt(const std::vector<uint8_t>& data) {
	// 초기 상태 설정 
	state = vector<vector<uint8_t>>(4, vector<uint8_t>(4)); 
	for (int i = 0; i < data.size(); ++i) 
		state[i / 4][i % 4] = data[i];

	// 복호화 
	addRoundKey(10);
	for (int round = 9; round > 0; --round) {
		invShiftRows();
		invSubBytes();
		addRoundKey(round);
		invMixColumns();
	}
	invShiftRows();
	invSubBytes();
	addRoundKey(0);
	
	// 결과 리턴
	vector<uint8_t>decryptedData(16);
	for (int i = 0; i < 16; ++i) 
		decryptedData[i] = state[i / 4][i % 4];
	return decryptedData;
}