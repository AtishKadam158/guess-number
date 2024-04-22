#ifndef CLIENT_H
#define CLIENT_H

#include "utilities.h"

class Client {
public:
    Client();
    void connectToServer();
    void sendMessage(const std::string& message);
    std::string receiveMessage();
    void closeConnection();
private:
    int sockfd;
};

#endif
