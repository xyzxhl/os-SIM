#include "../include/Handler.h"
#include "../../common/include/ComState.h"
#include <iostream>
#include <string>

using namespace std;

Handler::Handler(Database *db, Server *ser, int cliSocket)
    : database(db),
      server(ser),
      clientSocket(cliSocket)
{
}

void Handler::Handle()
{
    online = true;
    while (online)
    {
        ComState opt = ComState(server->Receive(clientSocket)[0]);
        switch (opt)
        {
        case ComState::LOGIN:
            Login();
            break;

        case ComState::EXIT:
            Exit();
            break;

        case ComState::PRINT_COURSE:
            PrintCourse();
            break;

        case ComState::PRINT_MEMBER:
            PrintMember();
            break;

        case ComState::ADD_USER:
            AddUser();
            break;

        case ComState::DELETE_USER:
            DeleteUser();
            break;

        case ComState::ADD_COURSE:
            AddCourse();
            break;

        case ComState::DELETE_COURSE:
            DeleteCourse();
            break;

        default:
            break;
        }

        if (ComState(server->Receive(clientSocket)[0]) != ComState::TASK_END)
        {
            cerr << "Failed to operate " << int(opt) << " in " << clientSocket << endl;
        }
    }
}

void Handler::Login()
{
    SendAccept();

    string username = server->Receive(clientSocket);
    if (username == "")
    {
        SendError();
        return;
    }

    server->Send(ComState::SUCCESS_RECV, clientSocket);

    string password = server->Receive(clientSocket);
    if (password == "")
    {
        SendError();
        return;
    }

    ComState role = database->CheckLogin(username, password);
    if (role == ComState::ERROR)
    {
        SendError();
        return;
    }

    cout << "Login in: " << username << endl;
    server->Send(role, clientSocket);
}

void Handler::Exit()
{
    SendAccept();
    online = false;
}

void Handler::PrintCourse()
{
    SendAccept();
}

void Handler::PrintMember()
{
    SendAccept();
}

void Handler::AddUser()
{
    SendAccept();

    string username = server->Receive(clientSocket);
    if (username == "")
    {
        SendError();
        return;
    }

    server->Send(ComState::SUCCESS_RECV, clientSocket);

    string password = server->Receive(clientSocket);
    if (password == "")
    {
        SendError();
        return;
    }

    server->Send(ComState::SUCCESS_RECV, clientSocket);

    ComState role = ComState(server->Receive(clientSocket)[0]);
    if (role == ComState::ERROR)
    {
        SendError();
        return;
    }

    database->AddUser(username, password, role);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}

void Handler::DeleteUser()
{
    SendAccept();

    string username = server->Receive(clientSocket);
    if (username == "")
    {
        SendError();
        return;
    }

    database->DeleteUser(username);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}

void Handler::AddCourse()
{
    SendAccept();

    string username = server->Receive(clientSocket);
    if (username == "")
    {
        SendError();
        return;
    }

    database->AddCourse(username);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}

void Handler::DeleteCourse()
{
    SendAccept();

    string username = server->Receive(clientSocket);
    if (username == "")
    {
        SendError();
        return;
    }

    database->DeleteCourse(username);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}