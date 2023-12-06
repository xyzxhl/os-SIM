#pragma once

#include "Database.h"
#include "Server.h"

#define SEND_ERROR_AND_END                           \
    {                                                \
        server->Send(ComState::ERROR, clientSocket); \
        online = false;                              \
        return;                                      \
    }

class Handler
{
public:
    Handler(Database *db, Server *ser, int cliSocket);

    void Handle();

    void Login();
    void Exit();

    void PrintCourse();
    void PrintMember();

    void AddUser();
    void DeleteUser();

    void AddCourse();
    void DeleteCourse();

private:
    Database *database;
    Server *server;
    int clientSocket;
    bool online;
};