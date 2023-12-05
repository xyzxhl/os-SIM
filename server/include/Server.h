#pragma once

#include "Database.h"
#include <thread>

class Server
{
    friend class Handler;

public:
    Server(int port);
    ~Server();

    void RunServer();
    void StopServer();

private:
    void StartAccepting();
    void HandleClient(int clientSocket);

    void Send(std::string content, int clientSocket);
    void Send(ComState state, int clientSocket);
    std::string Receive(int clientSocket);

private:
    int port;
    volatile int serverSocket;
    std::thread acceptingThread;
    Database database;
};