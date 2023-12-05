#pragma once

#include "Database.h"
#include "Server.h"

class Handler
{
public:
    Handler(Database *db, Server *ser, int cliSocket);

private:
    void SendAccept() { server->Send(ComState::ACCEPT_REQ, clientSocket); }
    void SendError()
    {
        server->Send(ComState::ERROR, clientSocket);
        online = false;
    }

public:
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