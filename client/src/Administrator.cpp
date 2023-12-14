#include "../include/SIM.h"
#include <iostream>

using namespace std;

void SIM::AddUser()
{
    cli->Send(ComState::ADD_USER);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("username");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("password");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    int r = 0;
    cout << "role(1:Administrator, 2:Teacher, 3:Student):";
    cin >> r;
    switch (r)
    {
    case 1:
        cli->Send(ComState::ADMINISTRATOR);
        break;

    case 2:
        cli->Send(ComState::TEACHER);
        break;

    case 3:
        cli->Send(ComState::STUDENT);
        break;

    default:
        cli->Send(ComState::ERROR);
        break;
    }
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}

void SIM::DeleteUser()
{
    cli->Send(ComState::DELETE_USER);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("username");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}

void SIM::AddCourse()
{
    cli->Send(ComState::ADD_COURSE);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}

void SIM::DeleteCourse()
{
    cli->Send(ComState::DELETE_COURSE);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}

void SIM::Backup()
{
    cli->Send(ComState::BACKUP);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}

void SIM::Recovery()
{
    cli->Send(ComState::RECOVERY);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}