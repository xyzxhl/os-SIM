#include "../include/Handler.h"
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

        if (online)
            if (ComState(server->Receive(clientSocket)[0]) != ComState::TASK_END)
            {
                online = false;
                cerr << "Failed to operate " << int(opt) << " in " << clientSocket << endl;
            }
    }
}

void Handler::Login()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);

    string username = server->Receive(clientSocket);
    if (username == "")
        SEND_ERROR_AND_END

    server->Send(ComState::SUCCESS_RECV, clientSocket);

    string password = server->Receive(clientSocket);
    if (password == "")
        SEND_ERROR_AND_END

    ComState role = database->CheckLogin(username, password);
    if (role == ComState::ERROR)
        SEND_ERROR_AND_END

    cout << "Login in: " << username << endl;
    server->Send(role, clientSocket);
}

void Handler::Exit()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);
    online = false;
}

void Handler::PrintCourse()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);
}

void Handler::PrintMember()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);
}

void Handler::AddUser()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);

    string username = server->Receive(clientSocket);
    if (username == "")
        SEND_ERROR_AND_END

    server->Send(ComState::SUCCESS_RECV, clientSocket);

    string password = server->Receive(clientSocket);
    if (password == "")
        SEND_ERROR_AND_END

    server->Send(ComState::SUCCESS_RECV, clientSocket);

    ComState role = ComState(server->Receive(clientSocket)[0]);
    if (role == ComState::ERROR)
        SEND_ERROR_AND_END

    database->AddUser(username, password, role);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}

void Handler::DeleteUser()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);

    string username = server->Receive(clientSocket);
    if (username == "")
        SEND_ERROR_AND_END

    database->DeleteUser(username);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}

void Handler::AddCourse()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);

    string username = server->Receive(clientSocket);
    if (username == "")
        SEND_ERROR_AND_END

    database->AddCourse(username);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}

void Handler::DeleteCourse()
{
    server->Send(ComState::ACCEPT_REQ, clientSocket);

    string username = server->Receive(clientSocket);
    if (username == "")
        SEND_ERROR_AND_END

    database->DeleteCourse(username);
    server->Send(ComState::SUCCESS_RECV, clientSocket);
}