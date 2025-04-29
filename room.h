#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

class Room {
    private:
        std::string name;
        std::vector<int> clients;
        std::mutex room_mutex;
    
    public:
        Room(const std::string& roomName);  // not std::string&, and not missing const

        void addClient(int clientSocket);
        void removeClient(int clientSocket);
        void broadcastMessage(const std::string& message, int senderSocket);
        bool isEmpty();
        std::string getName();
};

#endif