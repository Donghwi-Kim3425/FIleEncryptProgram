#include <iostream>
#include <string>
#include <vector>
#include "aes.h"
#include "seed.h"
#include "rsa.h"
#include "file_io.h"


using namespace std;

int main() {
    string filePath, operation, key;
    
    cout << "Enter the path to the file: ";
    getline(cin, filePath);

    cout << "Do you want to encrypt or decrypt the file? (encrypt/decrypt): ";
    getline(cin, operation);

    cout << "Enter the encryption key: ";
}

