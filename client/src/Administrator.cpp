#include "../include/Role.h"
#include "../../common/include/ComState.h"

using namespace std;

void Administrator::AddUser(string username, string password, Role role)
{
    cli->Send(ComState::ADD_USER);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        SendError();
        return;
    }

    cli->Send(username);

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return;
    }

    cli->Send(password);

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return;
    }

    switch (role)
    {
    case Role::Administrator:
        cli->Send(ComState::ADMINISTRATOR);
        break;

    case Role::Teacher:
        cli->Send(ComState::TEACHER);
        break;

    case Role::Student:
        cli->Send(ComState::STUDENT);
        break;

    default:
        cli->Send(ComState::ERROR);
        break;
    }

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return;
    }

    SendEnd();
}

void Administrator::DeleteUser(string username)
{
    cli->Send(ComState::DELETE_USER);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        SendError();
        return;
    }

    cli->Send(username);

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return;
    }

    SendEnd();
}

void Administrator::AddCourse(string courseName)
{
    cli->Send(ComState::ADD_COURSE);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        SendError();
        return;
    }

    cli->Send(courseName);

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return;
    }

    SendEnd();
}

void Administrator::DeleteCourse(string courseName)
{
    cli->Send(ComState::DELETE_COURSE);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
    {
        SendError();
        return;
    }

    cli->Send(courseName);

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
    {
        SendError();
        return;
    }

    SendEnd();
}