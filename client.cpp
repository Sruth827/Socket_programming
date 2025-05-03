#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cmath>

using namespace std;

long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        //if exponent is odd, multiply base
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        //square the base
        base = (base * base) % mod;
        //divide exponent by 2 
        exp /= 2;
    }
    return result;
}

long long rsaEncrypt(long long message, long long e, long long N) {
    //Encrypt using the public key
    return modExp(message, e, N);
}

string xorEncryptDecrypt(const string& input, long long key) {
    string output = input;
    for (char& c : output) {
        c ^= key;
    }
    return output;
}

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // sending connection request
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection to server failed");
        return -1;
    }
    cout << "Connected to Server" << endl;

    long long e, N;
    recv(clientSocket, &e, sizeof(e), 0);
    recv(clientSocket, &N, sizeof(N), 0);
    
    long long symmetricKey = 16;
    cout << "Symmetric Key: " << symmetricKey << endl;
    long long encryptedKey = rsaEncrypt(symmetricKey, e, N);
    cout << "Encrypted Symmetric key: " << encryptedKey << endl;
    send(clientSocket, &encryptedKey, sizeof(encryptedKey), 0);
    cout << "Encrypted key sent to server" << encryptedKey << endl;

    string message;
    char buffer[1024];

    while (true) {
        cout << "Enter message -- type 'quit' to quit): ";
        getline(cin, message);
        if (message == "quit") {
            cout << "Quitting connection" << endl;
            break;
        }


        string encryptedMessage = xorEncryptDecrypt(message, symmetricKey);
        cout << "Client sending encrypted message: " << encryptedMessage << endl << endl;
        send(clientSocket, encryptedMessage.c_str(), encryptedMessage.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        string decryptedMessage = xorEncryptDecrypt(buffer, symmetricKey);
        cout << "Server response: " << buffer << endl;
        cout << "Decrypted response: " << decryptedMessage << endl;
    }

    // closing socket
    close(clientSocket);
    return 0;
}
