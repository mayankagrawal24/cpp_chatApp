#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>
#include <cstring>

#define PORT 8080

void receiveMessages(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t valread = read(sock, buffer, sizeof(buffer));
        if (valread <= 0) {
            std::cout << "\nDisconnected from server.\n";
            break;
        }
        std::cout << "\n[Server] " << buffer << "\n> ";
        std::cout.flush();
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to chat server.\n";
    std::cout << "Type /join <room>, /leave, or any message:\n";

    // Start thread to receive messages
    std::thread receiver(receiveMessages, sock);
    receiver.detach();

    std::string message;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, message);
        if (message == "/quit") {
            break;
        }
        send(sock, message.c_str(), message.length(), 0);
    }

    close(sock);
    return 0;
}
