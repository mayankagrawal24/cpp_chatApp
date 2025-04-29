#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#define PORT 8080

int main () {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    //create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
      std::cerr << "Invalid address/ Address not supported" << std::endl;
      return -1;
    }
  
    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      std::cerr << "Connection Failed" << std::endl;
      return -1;
    }
  
    // Communicate
    while (true) {
        std::string message;
        std::cout << "Client: ";
        std::getline(std::cin, message);
        send(sock, message.c_str(), message.length(), 0);

        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Server closed connection.\n";
            break;
        }
        std::cout << "Server: " << buffer << std::endl;
    }
  
    // Close socket
    close(sock);
    return 0;
}