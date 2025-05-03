#  Socket Programming with RSA Key Exchange & XOR Message Encryption 

## ⚠️ Disclaimer

This project is intended for **learning purposes only** and **should not be used in production**.  

### Security Considerations:
✔ The RSA implementation uses **small prime numbers**, making it **insecure for real-world encryption**.  
✔ The **XOR-based encryption** is a basic approach and **not suitable for strong security**.  
✔ Messages are **logged in plaintext**, which is **not recommended for actual encrypted communication**.  
✔ The symmetric key is **hardcoded** and should be **randomly generated per session** in secure implementations.  

🚀 If you plan to use encryption for real-world applications, consider **implementing AES or RSA with proper key management**.

### Overview
This project implements a **client-server socket communication** with **RSA key exchange** for secure connection establishment, **XOR encryption** for message security, and **MySQL integration** to store network traffic. This will continue to be my repo for experimenting with socket communication and encryption methods in the future.

### Features
**Secure RSA Key Exchange** for symmetric key transfer  
**XOR-based Message Encryption** for lightweight security  
**MySQL Integration** for storing transmitted messages  
**Multi-client support** for real-world scalability  

### Planned Improvements
 **Multithreading** for Client Handling / **AES**-based Encryption / **WebSocket** Integration

---

##  Setup & Installation

### ** Prerequisites**
- **C++ Compiler** (`g++`)
- **MySQL Server**
- **MySQL Connector/C++ (for MySQL X API)**
- **Git** (for version control)

### Running on WSL
This project was developed and tested in **WSL (Windows Subsystem for Linux)**.  
If you're using WSL, ensure that MySQL Server is installed **inside WSL**, not on Windows.

### ** Install Dependencies**
```sh
sudo apt update && sudo apt install g++ mysql-server libmysqlcppconn-dev

g++ server.cpp -o server -I/usr/include/mysqlx -L/usr/lib -lmysqlcppconn -lmysqlcppconnx
./server

g++ client.cpp -o client
./client
