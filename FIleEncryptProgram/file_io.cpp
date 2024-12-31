#include "file_io.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

// ���� �б� �Լ� (���̳ʸ�)
vector<uint8_t> readFile(const string& filePath) {
    try {
        // ������ ���̳ʸ� ���� �б�
        ifstream file(filePath, ios::binary);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            throw runtime_error("Could not open file for reading");
        }

        // ���� ũ�� Ȯ��
        file.seekg(0, ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, ios::beg);

        if (fileSize == 0) {
            cerr << "Warning: File is empty: " << filePath << endl;
        }

        // ���� ������ vector�� ����
        vector<uint8_t> fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

        if (file.fail()) {
            cerr << "Error occurred while reading file: " << filePath << endl;
            throw runtime_error("File read operation failed");
        }

        file.close();

        cout << "Successfully read " << fileContents.size() << " bytes from " << filePath << endl;
        cout << "File content (hex): ";
        for (size_t i = 0; i < min(fileContents.size(), size_t(32)); ++i) {
            printf("%02x ", fileContents[i]);
            if (i % 16 == 15) cout << endl;
        }
        if (fileContents.size() > 32) cout << "... (truncated)";
        cout << endl;

        return fileContents;
    }
    catch (const exception& e) {
        cerr << "Error in readFile: " << e.what() << endl;
        throw;
    }
}

// ���� �б� �Լ� (16����)
vector<uint8_t> readHexFile(const string& filePath) {
    try {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Failed to open hex file: " << filePath << endl;
            throw runtime_error("Could not open hex file for reading");
        }

        string hexString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        // 16���� ���ڿ� ��ȿ�� �˻�
        if (hexString.empty()) {
            throw runtime_error("Hex file is empty");
        }
        if (hexString.length() % 2 != 0) {
            throw runtime_error("Invalid hex string length - must be even");
        }

        cout << "Successfully read hex file: " << filePath << endl;
        return hexToBytes(hexString);
    }
    catch (const exception& e) {
        cerr << "Error in readHexFile: " << e.what() << endl;
        throw;
    }
}

// ���� ���� �Լ� (16����)
void writeFile(const string& filePath, const vector<uint8_t>& data) {
    try {
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "Failed to open file for writing: " << filePath << endl;
            throw runtime_error("Could not open file for writing");
        }

        string hexString = bytesToHex(data);
        file << hexString;

        if (file.fail()) {
            cerr << "Error occurred while writing to file: " << filePath << endl;
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

// ���� ���� �Լ� (string) 
void writeStringFile(const string& filePath, const string& data) {
    try {
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "Failed to open file for writing: " << filePath << endl;
            throw runtime_error("Could not open file for writing");
        }

        file << data;

        if (file.fail()) {
            cerr << "Error occurred while writing to file: " << filePath << endl;
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

// ����Ʈ �����͸� 16���� ���ڿ��� ��ȯ
string bytesToHex(const vector<uint8_t>& bytes) {
    try {
        if (bytes.empty()) {
            cerr << "Warning: Converting empty byte array to hex" << endl;
        }

        stringstream ss;
        ss << hex << uppercase;
        for (uint8_t byte : bytes) {
            ss << setw(2) << setfill('0') << static_cast<int>(byte);
        }
        return ss.str();
    }
    catch (const exception& e) {
        cerr << "Error in bytesToHex: " << e.what() << endl;
        throw;
    }
}

// 16���� ���ڿ��� ����Ʈ�� �����ͷ� ��ȯ
vector<uint8_t> hexToBytes(const string& hex) {
    try {
        if (hex.empty()) {
            throw runtime_error("Empty hex string");
        }
        if (hex.length() % 2 != 0) {
            throw runtime_error("Invalid hex string length - must be even");
        }

        vector<uint8_t> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            string byteString = hex.substr(i, 2);

            // 16���� ���� ��ȿ�� �˻�
            if (!isxdigit(byteString[0]) || !isxdigit(byteString[1])) {
                throw runtime_error("Invalid hex character at position " + to_string(i));
            }

            uint8_t byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }
    catch (const exception& e) {
        cerr << "Error in hexToBytes: " << e.what() << endl;
        throw;
    }
}

// ����Ʈ �����͸� ���ڿ��� ��ȯ 
string bytesToString(const vector<uint8_t>& bytes) {
    try {
        if (bytes.empty()) {
            cerr << "Warning: Converting empty byte array to string" << endl;
        }
        return string(bytes.begin(), bytes.end());
    }
    catch (const exception& e) {
        cerr << "Error in bytesToString: " << e.what() << endl;
        throw;
    }
}

string toHexString(const vector<unsigned long long>& data) {
    stringstream ss;
    for (const auto& num : data) {
        ss << hex << setw(16) << setfill('0') << num << " ";
    }
    return ss.str();
}

vector<unsigned long long> fromHexString(const string& hexStr) {
    vector<unsigned long long> result;
    stringstream ss(hexStr);
    string item;
    while (getline(ss, item, ' ')) {
        if (!item.empty()) {
            result.push_back(stoull(item, nullptr, 16));
        }
    }
    return result;
}