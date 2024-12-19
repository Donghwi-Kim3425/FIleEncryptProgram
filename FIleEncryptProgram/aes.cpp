#include "aes.h"
#include <vector>
#include <cstdint>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

AES::AES(const std::string& keyHex) { // 16진수 문자열을 바이트 벡터로 변환 
	vector<uint8_t> keyBytes; 
	for (size_t i = 0; i < keyHex.length(); i += 2)
		keyBytes.push_back(static_cast<uint8_t>(stoi(keyHex.substr(i, 2), nullptr, 16)));  
	this->key = keyBytes; 
	keyExpansion();
}

string AES::generateRandomKey() {
	const int keyLength = 16; // AES-128은 16바이트의 키 (128 비트)
	vector<uint8_t> key;
	key.reserve(keyLength);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 255);

	for (int i = 0; i < keyLength; ++i)
		key.push_back(static_cast<uint8_t>(dis(gen)));

	stringstream ss;
	for (int i = 0; i < keyLength; ++i)
		ss << hex << setw(2) << setfill('0') << static_cast<int>(key[i]);

	return ss.str();
}

// 패딩 추가 함수
vector<uint8_t> AES::padData(const vector<uint8_t>& data) {
	vector<uint8_t> paddedData = data;
	size_t padLength = 16 - (data.size() % 16);
	paddedData.insert(paddedData.end(), padLength, static_cast<uint8_t>(padLength));
	return paddedData;
}

// 패딩 제거 함수
vector<uint8_t> AES::unpadData(const vector<uint8_t>& data) {
	if (data.empty()) return data;
	size_t padLength = data.back();
	return vector<uint8_t>(data.begin(), data.end() - padLength);
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

// GF(2^8) 곱셈 함수
uint8_t AES::gmul(uint8_t a, uint8_t b) {
	uint8_t p = 0;
	for (int i = 0; i < 8; i++) {
		if (b & 1)
			p ^= a;
		bool hi_bit_set = (a & 0x80);
		a <<= 1;
		if (hi_bit_set)
			a ^= 0x1B; // x^8 + x^4 + x^3 + x + 1
		b >>= 1;
	}
	return p;
}

const uint8_t AES::Sbox[256] = {
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
	0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
	0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
	0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
	0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
	0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
	0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
	0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
	0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
	0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
	0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
	0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
	0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
	0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
	0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
	0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
	0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

const uint8_t AES::InvSbox[256] = {
	0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38,
	0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87,
	0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D,
	0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2,
	0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16,
	0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA,
	0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A,
	0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02,
	0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA,
	0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
	0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89,
	0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20,
	0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31,
	0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D,
	0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0,
	0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26,
	0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

const uint8_t AES::Rcon[11]{
	0xFF, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

vector<uint8_t> AES::encrypt(const vector<uint8_t>& data) {
	// 데이터 패딩
	vector<uint8_t> paddedData = padData(data);
	
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
	
	// 패딩 제거 후 결과 리턴
	vector<uint8_t>decryptedData(16);
	for (int i = 0; i < 16; ++i) 
		decryptedData[i] = state[i / 4][i % 4];
	return unpadData(decryptedData);
}