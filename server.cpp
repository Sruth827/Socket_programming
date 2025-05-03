#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysqlx/xdevapi.h>
#include <cmath>
#include <string>

using namespace std;

void generatePublicKey(long long p, long long q, long long &e, long long &N) {
    N = p * q; 
    e = 5;
}


long long generatePrivateKey(long long e, long long phi) {
    long long d = 1;
    //Mod inverse 
    while ((d * e) % phi != 1) {
        d++;
    }
    return d; 
}


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

long long rsaDecrypt(long long encryptedMessage, long long d, long long N) {
    //Decrypt using RSA key - private
    return modExp(encryptedMessage, d, N) ; 
}

std::string xorEncryptDecrypt(const string& input, long long key) {
    string output = input; 
    for (char& c : output) {
        c ^= key;
    }
    return output; 
}


void storeMessage(const string& sender, const string& content) {
    //Establish MySQL8.0 Ubuntu Connection using mysqlxdevapi 
    try {
        mysqlx::Session sess("mysqlx://root:****@127.0.0.1:33060");
        cout << "Session accepted " << endl;
        mysqlx::Schema chatDB = sess.getSchema("chat_app");
        mysqlx::Table messages = chatDB.getTable("messages");

        //insert message traffic into SQL Table 'messages'
        messages.insert("sender", "content").values(sender, content).execute();
        cout << "Message stored" << endl;
    }
    catch (const mysqlx::Error& err) {
        cerr << "MySQL X error: " << err.what() << endl;
    }
}


int main(int argc, const char* argv[])
{   

    


    // Server socket creation 
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Client socket creation failed");
        return -1;
    }


    // Specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Binding socket.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Failed to bind");
        return -1;
    }

    // Listening to socket
    listen(serverSocket, 5);
    cout << "Port 8080 / Server listening..." << endl;

    // Accepting connection request
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        perror("Client 'accept' failed");
        return -1;
    }
    cout << "Client connected" << endl;

    //Values used for RSA key sharing 
    long long p = 7, q = 13;
    long long e, N;
    generatePublicKey(p, q, e, N);
    long long phi = (p - 1) * (q - 1);
    long long d = generatePrivateKey(e, phi);

    //Sending client the public key (e, N)
    send(clientSocket, &e, sizeof(e), 0);
    send(clientSocket, &N, sizeof(N), 0);
    
    //Server recieves ecrypted symmetric key
    long long encryptedKey; 
    recv(clientSocket, &encryptedKey, sizeof(encryptedKey), 0);
    cout << "Encrypted symmetric key received: " << encryptedKey << endl;
    //Server decrypts symmetric using private key (d, N)
    long long decryptedKey = rsaDecrypt(encryptedKey, d, N);
    cout << "Decrypted symmetric key: " << decryptedKey << endl;


    char buffer[1024] = { 0 };

    while (true) {
        //Clear buffer
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        buffer[bytesReceived] = '\0';  // Ensure null termination
        if (bytesReceived <= 0) {
            cout << "Disconnected" << endl;
            break;
        }

        //Print message traffic for troubleshooting
        cout << "Message from client: " << buffer << endl;
        string decryptedMessage = xorEncryptDecrypt(buffer, decryptedKey);
        cout << "Decrypted Message : " << decryptedMessage << endl;
        storeMessage("client", decryptedMessage);


        send(clientSocket, decryptedMessage.c_str(), decryptedMessage.size() + 1 , 0);

    }

    close(clientSocket);
    close(serverSocket);


    return 0;
}
