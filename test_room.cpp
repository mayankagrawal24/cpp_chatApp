#include "room.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Create a room called "TestRoom"
    Room myRoom("TestRoom");

    // Fake client socket numbers (pretend 1, 2, 3 are client sockets)
    myRoom.addClient(1);
    myRoom.addClient(2);
    myRoom.addClient(3);

    std::cout << "Room '" << myRoom.getName() << "' created.\n";
    
    // Broadcast a test message from client 1
    myRoom.broadcastMessage("Hello from client 1!", 1);

    // Remove client 2
    myRoom.removeClient(2);

    // Check if the room is empty
    if (myRoom.isEmpty()) {
        std::cout << "Room is empty.\n";
    } else {
        std::cout << "Room still has clients.\n";
    }

    return 0;
}
