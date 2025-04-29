# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Source files
SERVER_SRC = server.cpp Room.cpp
CLIENT_SRC = client.cpp

# Output executables
SERVER_BIN = server
CLIENT_BIN = client

# Build all targets
all: $(SERVER_BIN) $(CLIENT_BIN)

# Server build
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN) -lpthread

# Client build
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN) -lpthread

# Clean up
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
