#include "client.h"
#include "constants.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
Client::Client() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        error("WSAStartup failed");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        error("Error creating socket");
    }
}

void Client::connectToServer() {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        error("Error connecting to server");
    }
}

void Client::sendMessage(const std::string& message) {
    send(sockfd, message.c_str(), message.length(), 0);
}

std::string Client::receiveMessage() {
    char buffer[1024];
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        error("Error receiving data from server");
    }
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}

void Client::closeConnection() {
    closesocket(sockfd);
    WSACleanup();
}

int main() {
    Client client;
    client.connectToServer();
    std::cout << "Connected to server." << std::endl;

    // Number guessing game
    std::string hint = client.receiveMessage();
    std::cout << "Server says: " << hint << std::endl;

    int guess;
    while (true) {
        std::cout << "Enter your guess: ";
        std::cin >> guess;
        client.sendMessage(std::to_string(guess));

        std::string response = client.receiveMessage();
        std::cout << "Server says: " << response << std::endl;

        if (response == "Congratulations! You guessed the correct number.") {
            break;
        }
    }

    client.closeConnection();
    std::cout << "Connection closed." << std::endl;

    return 0;
}
