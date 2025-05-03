#  Socket Programming with RSA Key Exchange & XOR Message Encryption 

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

### **🔹 Install Dependencies**
```sh
sudo apt update && sudo apt install g++ mysql-server libmysqlcppconn-dev

g++ server.cpp -o server -I/usr/include/mysqlx -L/usr/lib -lmysqlcppconn -lmysqlcppconnx
./server

g++ client.cpp -o client
./client
