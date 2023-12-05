#pragma once

#include "../../common/include/ComState.h"
#include <string>

class Client
{
public:
    Client(int port);
    ~Client();

    void Connect();
    void Disconnect();

    void Send(std::string content);
    void Send(ComState state);
    std::string Receive();

private:
    int port;
    int clientSocket;
};