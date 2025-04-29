#include "room.h"
#include <algorithm>    // for std::remove
#include <sys/socket.h> // for send()
#include <unistd.h>     // for close()
#include <iostream>     // for std::cerr

Room::Room(const std::string& roomName) : name(roomName) {}


void Room::addClient(int clientSocket) {
    std::lock_guard<std::mutex> lock(room_mutex);
    clients.push_back(clientSocket);
}

void Room::removeClient(int clientSocket) {
    std::lock_guard<std::mutex> lock(room_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());

}

void Room::broadcastMessage(const std::string& message, int senderSocket) {
    std::lock_guard<std::mutex> lock(room_mutex);

    for (int client: clients) {
        if (client != senderSocket) {
            ssize_t result = send(client, message.c_str(), message.length(), 0);
            if (result == -1) {
                std::cerr << "Failed to send message to client socket: " << client << std::endl;
            }
        }
    }
}

bool Room::isEmpty() {
    std::lock_guard<std::mutex> lock(room_mutex);
    return clients.empty();
}

std::string Room :: getName() {
    return name;
}