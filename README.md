#  Multi-Client Room-Based Chat Server (C++)

This is a simple terminal-based TCP chat server and client built in C++. It supports:

- Multiple simultaneous clients (via threads)
- Room creation and joining (`/join <room>`)
- Leaving rooms (`/leave`)
- Broadcasting messages to other clients in the same room
- Clean handling of disconnections
- `/quit` command for graceful client exit


## Requirements

- C++17 or later
- POSIX-compatible system (Linux, WSL, macOS)
- `g++` compiler


## Build Instructions

Run the following in your terminal (WSL or Linux preferred):

```bash
make


## Run Instructions
Do the following command to start the server
./server

Do the following command on each client to start it
./client




