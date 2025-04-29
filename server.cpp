#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <memory>  
#include "room.h"

#define PORT 8080

// Use shared_ptr to avoid copying Room (mutex is non-copyable)
std::map<std::string, std::shared_ptr<Room>> rooms;
std::map<int, std::string> clientToRoom;
std::mutex global_mutex;

void clientHandler(int clientSocket) {
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) break;

        std::string msg(buffer);
        msg.erase(msg.find_last_not_of("\r\n") + 1); // Trim newlines

        std::lock_guard<std::mutex> lock(global_mutex);

        if (msg.rfind("/join ", 0) == 0) {
            std::string roomName = msg.substr(6);

            // Leave previous room if needed
            if (clientToRoom.count(clientSocket)) {
                std::string oldRoom = clientToRoom[clientSocket];
                rooms[oldRoom]->removeClient(clientSocket);
                if (rooms[oldRoom]->isEmpty()) {
                    rooms.erase(oldRoom);
                }
            }

            // Join or create new room
            if (!rooms.count(roomName)) {
                rooms[roomName] = std::make_shared<Room>(roomName);
            }
            rooms[roomName]->addClient(clientSocket);
            clientToRoom[clientSocket] = roomName;

            std::string joinedMsg = "You joined room: " + roomName + "\n";
            send(clientSocket, joinedMsg.c_str(), joinedMsg.length(), 0);

        } else if (msg == "/leave") {
            if (clientToRoom.count(clientSocket)) {
                std::string room = clientToRoom[clientSocket];
                rooms[room]->removeClient(clientSocket);
                clientToRoom.erase(clientSocket);
                if (rooms[room]->isEmpty()) {
                    rooms.erase(room);
                }
                std::string leaveMsg = "You left the room.\n";
                send(clientSocket, leaveMsg.c_str(), leaveMsg.length(), 0);
            } else {
                std::string info = "You are not in a room.\n";
                send(clientSocket, info.c_str(), info.length(), 0);
            }

        } else {
            // Normal message → broadcast to room
            if (clientToRoom.count(clientSocket)) {
                std::string room = clientToRoom[clientSocket];
                std::string fullMessage = "Client " + std::to_string(clientSocket) + ": " + msg;
                rooms[room]->broadcastMessage(fullMessage, clientSocket);
            } else {
                std::string warn = "You must /join a room first.\n";
                send(clientSocket, warn.c_str(), warn.length(), 0);
            }
        }
    }

    // Handle disconnection
    {
        std::lock_guard<std::mutex> lock(global_mutex);
        if (clientToRoom.count(clientSocket)) {
            std::string room = clientToRoom[clientSocket];
            rooms[room]->removeClient(clientSocket);
            clientToRoom.erase(clientSocket);
            if (rooms[room]->isEmpty()) {
                rooms.erase(room);
            }
        }
    }

    close(clientSocket);
    std::cout << "Client " << clientSocket << " disconnected.\n";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        std::cout << "New client connected: " << new_socket << "\n";
        std::thread(clientHandler, new_socket).detach();
    }

    close(server_fd);
    return 0;
}
