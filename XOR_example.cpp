#include <iostream>
#include <string>

using namespace std;

string xorEncryptDecrypt(const string& input, char key) {
    string output = input;
    for (char& c : output) {
        c ^= key;  // XOR operation
    }
    return output;
}

int main() {
    string message = "Hello, World!";
    char key = 'K';  // Simple key for XOR encryption

    // Encrypt the message
    string encrypted = xorEncryptDecrypt(message, key);
    cout << "Encrypted: " << encrypted << endl;

    // Decrypt the message (same function)
    string decrypted = xorEncryptDecrypt(encrypted, key);
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}