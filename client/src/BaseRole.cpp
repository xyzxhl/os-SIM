#include "../include/Role.h"
#include "../../common/include/ComState.h"
#include "iostream"

using namespace std;

BaseRole::BaseRole(FileSystem *fileSystem, Client *client)
    : fileSys(fileSystem),
      cli(client)
{
}

Role BaseRole::Login(string username, string password)
{
    cli->Connect();

    cli->Send(ComState::LOGIN);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        SendError();
        return Role::Unknown;
    }

    cli->Send(username);

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return Role::Unknown;
    }

    cli->Send(password);

    ComState opt = ComState(cli->Receive()[0]);
    Role res;
    switch (opt)
    {
    case ComState::ADMINISTRATOR:
        res = Role::Administrator;
        break;

    case ComState::TEACHER:
        res = Role::Teacher;
        break;

    case ComState::STUDENT:
        res = Role::Student;
        break;

    default:
        SendError();
        res = Role::Unknown;
        return res;
    }

    SendEnd();

    return res;
}

void BaseRole::Interact()
{
    online = true;
    while (online)
    {
    }
}

void BaseRole::Exit()
{

    cli->Send(ComState::EXIT);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        cout << "Fail to send request.";
        SendError();
        cli->Disconnect();
        return;
    }

    SendEnd();
    cli->Disconnect();
    cout << "Successfully disconnect." << endl;
}

void BaseRole::PrintCourse()
{
    cli->Send(ComState::PRINT_COURSE);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        cout << "Fail to send request.";
        SendError();
        return;
    }

    cli->Send(ComState::SUCCESS_RECV);

    cout << cli->Receive();

    SendEnd();
}

void BaseRole::PrintMember()
{
    cli->Send(ComState::PRINT_MEMBER);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        cout << "Fail to send request.";
        SendError();
        return;
    }

    cli->Send(ComState::SUCCESS_RECV);

    cout << cli->Receive();

    SendEnd();
}