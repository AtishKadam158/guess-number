#include "server.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#include <ws2tcpip.h>


Server::Server() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        error("WSAStartup failed");
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        error("Error creating socket");
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        error("Error binding socket");
    }
}

void Server::startListening() {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        error("Error listening on socket");
    }
}

void Server::acceptConnection() {
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        error("Error accepting connection");
    }
}

std::string Server::receiveMessage() {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        error("Error receiving data from client");
    } else if (bytesReceived == 0) {
        error("Connection closed by client");
    }
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}

void Server::sendMessage(const std::string& message) {
    send(clientSocket, message.c_str(), message.length(), 0);
}

void Server::closeConnection() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    Server server;
    server.startListening();
    server.acceptConnection();

    // Generate a random number for the client to guess
    std::srand(std::time(nullptr));
    int targetNumber = std::rand() % 100 + 1;

    // Send initial hint to the client
    server.sendMessage("Guess a number between 1 and 100.");

    while (true) {
        // Receive guess from client
        std::string clientGuessStr = server.receiveMessage();
        int clientGuess = std::stoi(clientGuessStr);

        // Evaluate client's guess
        if (clientGuess == targetNumber) {
            server.sendMessage("Congratulations! You guessed the correct number.");
            break;
        } else if (clientGuess < targetNumber) {
            server.sendMessage("Too low. Try guessing higher.");
        } else {
            server.sendMessage("Too high. Try guessing lower.");
        }
    }

    server.closeConnection();
    return 0;
}
