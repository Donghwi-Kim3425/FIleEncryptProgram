#include "file_io.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

// 파일 읽기 함수 (바이너리)
vector<uint8_t> readFile(const string& filePath) {
    try {
        // 파일을 바이너리 모드로 읽기
        ifstream file(filePath, ios::binary);
        if (!file.is_open()) {
            throw runtime_error("Could not open file for reading: " + filePath);
        }

        // 파일 크기 확인
        file.seekg(0, ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, ios::beg);

        if (fileSize == 0) {
            cerr << "Warning: File is empty: " << filePath << endl;
        }

        // 데이터 읽기
        vector<uint8_t> data(fileSize);
        file.read(reinterpret_cast<char*>(data.data()), fileSize);

        if (file.fail()) {
            throw runtime_error("File read operation failed");
        }

        file.close();

        cout << "Successfully read " << data.size() << " bytes from " << filePath << endl;
        cout << "File content (hex): ";
        for (size_t i = 0; i < min(data.size(), size_t(16)); ++i) {
            printf("%02x ", data[i]);
            if (i % 16 == 15) cout << endl;
        }
        if (data.size() > 16) cout << "... (truncated)";
        cout << endl;

        return data;
    }
    catch (const exception& e) {
        cerr << "Error in readFile: " << e.what() << endl;
        throw;
    }
}

// 파일 쓰기 함수 (바이너리)
void writeFile(const string& filePath, const vector<uint8_t>& data) {
    try {
        ofstream file(filePath, ios::binary);
        if (!file.is_open()) {
            throw runtime_error("Could not open file for writing: " + filePath);
        }

        file.write(reinterpret_cast<const char*>(data.data()), data.size());

        if (file.fail()) {
            throw runtime_error("File write operation failed");
        }

        file.close();
        cout << "Successfully wrote " << data.size() << " bytes to " << filePath << endl;
    }
    catch (const exception& e) {
        cerr << "Error in writeFile: " << e.what() << endl;
        throw;
    }
}

// 문자열 파일 쓰기 함수 (다이제스트 등을 위해 유지)
void writeStringFile(const string& filePath, const string& data) {
    try {
        ofstream file(filePath);
        if (!file.is_open()) {
            throw runtime_error("Could not open file for writing: " + filePath);
        }

        file << data;

        if (file.fail()) {
            throw runtime_error("File write operation failed");
        }

        file.close();
        cout << "Successfully wrote " << data.length() << " characters to " << filePath << endl;
    }
    catch (const exception& e) {
        cerr << "Error in writeStringFile: " << e.what() << endl;
        throw;
    }
}

// 문자열 파일 읽기 함수
string readStringFile(const string& filePath) {
    try {
        ifstream file(filePath);
        if (!file.is_open()) {
            throw runtime_error("Could not open file for reading: " + filePath);
        }

        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return content;
    }
    catch (const exception& e) {
        cerr << "Error in readStringFile: " << e.what() << endl;
        throw;
    }
}

// 유틸리티 함수들 (그대로 유지)
string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;
    ss << hex << uppercase;
    for (uint8_t byte : bytes) {
        ss << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

vector<uint8_t> hexToBytes(const string& hex) {
    if (hex.empty() || hex.length() % 2 != 0) {
        throw runtime_error("Invalid hex string");
    }

    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        if (!isxdigit(byteString[0]) || !isxdigit(byteString[1])) {
            throw runtime_error("Invalid hex character");
        }
        bytes.push_back(static_cast<uint8_t>(stoi(byteString, nullptr, 16)));
    }
    return bytes;
}

string bytesToString(const vector<uint8_t>& bytes) {
    return string(bytes.begin(), bytes.end());
}

string toHexString(const vector<unsigned long long>& data) {
    stringstream ss;
    for (const auto& num : data) {
        ss << hex << setw(16) << setfill('0') << num << " ";
    }
    return ss.str();
}

vector<unsigned long long> fromHexString(string hexStr) {
    vector<unsigned long long> result;
    istringstream ss(hexStr);
    string value;

    while (ss >> value) {
        value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
        if (value.substr(0, 2) == "0x") {
            value = value.substr(2);
        }
        result.push_back(stoull(value, nullptr, 16));
    }
    return result;
}