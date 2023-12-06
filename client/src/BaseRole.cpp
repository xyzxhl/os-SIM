#include "../include/SIM.h"
#include "iostream"

using namespace std;

SIM::SIM(FileSystem *fileSystem, Client *client)
    : fileSys(fileSystem),
      cli(client),
      role(Role::Unknown)
{
    options.push_back(make_pair("Login", &SIM::Login));
    options.push_back(make_pair("Exit", &SIM::Exit));
    options.push_back(make_pair("PrintCourse", &SIM::PrintCourse));
    options.push_back(make_pair("PrintMember", &SIM::PrintMember));
}

void SIM::Interact()
{
    online = true;
    cli->Connect();

    Login();
    switch (role)
    {
    case Role::Administrator:
        cout << "Successfully Login in. Welcome, administrator." << endl;
        break;

    case Role::Student:
        cout << "Successfully Login in. Welcome, student." << endl;
        break;

    case Role::Teacher:
        cout << "Successfully Login in. Welcome, teacher." << endl;
        break;

    default:
        cerr << "Fail to login" << endl;
        break;
    }

    while (online)
    {
        cout << "These are all optional actions, please choose from them:" << endl;
        for (int i = 0; i < options.size(); i++)
        {
            cout << options[i].first << endl;
        }
        string opt;
        cin >> opt;
        for (int i = 0; i < options.size(); i++)
        {
            if (opt == options[i].first)
            {
                (this->*(options[i].second))();
                break;
            }
        }
        cout << "Invalid action." << endl;
    }

    cli->Disconnect();
    cout << "Successfully disconnect." << endl;
}

void SIM::Login()
{
    cli->Send(ComState::LOGIN);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    ASK_AND_SEND(username)

    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    ASK_AND_SEND(password)

    ComState opt = ComState(cli->Receive()[0]);
    switch (opt)
    {
    case ComState::ADMINISTRATOR:
        role = Role::Administrator;
        break;

    case ComState::TEACHER:
        role = Role::Teacher;
        break;

    case ComState::STUDENT:
        role = Role::Student;
        break;

    default:
        role = Role::Unknown;
        SEND_ERROR_AND_END
    }

    cli->Send(ComState::TASK_END);
}

void SIM::Exit()
{

    cli->Send(ComState::EXIT);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
}

void SIM::PrintCourse()
{
    cli->Send(ComState::PRINT_COURSE);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    cli->Send(ComState::SUCCESS_RECV);

    cout << cli->Receive();

    cli->Send(ComState::TASK_END);
}

void SIM::PrintMember()
{
    cli->Send(ComState::PRINT_MEMBER);

    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    cli->Send(ComState::SUCCESS_RECV);

    cout << cli->Receive();

    cli->Send(ComState::TASK_END);
}