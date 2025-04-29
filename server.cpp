#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#define PORT 8080

int main() {    
    int server_fd, new_socket;
    struct sockaddr_in address;

    int opt = 1; 
    int addrlen = sizeof(address);
    char buffer[1024] = {0}; 

    //create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket server creation failed");
        return -1;
    }

    //attach socket to the port
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port: " << PORT << "...\n";

    // Accept
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket<0) {
            perror("accept");
            exit(EXIT_FAILURE);
    }

    while (true) {  
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread == 0) {
            std::cout << "Connection Closed.\n";
            break;
        } 
        
        std::cout << "Client: " << buffer << std::endl;

        std::string response;
        std::cout << "Server: ";
        std::getline(std::cin, response);
        send(new_socket, response.c_str(), response.length(), 0);

    }

    // Close socket
    close(new_socket);
    close(server_fd);
    return 0;  
}



