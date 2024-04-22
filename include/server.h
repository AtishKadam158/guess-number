#ifndef SERVER_H
#define SERVER_H

#include "utilities.h"

class Server {
public:
    Server();
    void startListening();
    void acceptConnection();
    std::string receiveMessage();
    void sendMessage(const std::string& message);
    void closeConnection();
private:
    int serverSocket;
    int clientSocket;
};

#endif
