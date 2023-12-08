#include "../include/SIM.h"
#include "iostream"

using namespace std;

SIM::SIM(FileSystem *fileSystem, Client *client)
    : fileSys(fileSystem),
      cli(client),
      role(Role::Unknown)
{
    ADD_OPTION(Exit)
    ADD_OPTION(PrintCourse)
    ADD_OPTION(PrintMember)
    options.push_back(make_pair("", nullptr));
}

void SIM::PrintOptions()
{
    cout << "\nThese are all optional actions, please choose from them:" << endl;
    for (int i = 0; i < options.size(); i++)
    {
        cout << options[i].first << endl;
    }
    cout << endl;
}

void SIM::AskAndSend(string q)
{
    string ss;
    cout << q << ":";
    cin >> ss;
    cli->Send(ss);
}

void SIM::Interact()
{
    online = true;
    cli->Connect();

    Login();
    switch (role)
    {
    case Role::Administrator:
        cout << "\nSuccessfully Login in. Welcome, administrator." << endl;
        PrintOptions();
        break;

    case Role::Student:
        cout << "\nSuccessfully Login in. Welcome, student." << endl;
        PrintOptions();
        break;

    case Role::Teacher:
        cout << "\nSuccessfully Login in. Welcome, teacher." << endl;
        PrintOptions();
        break;

    default:
        cerr << "Failed to login" << endl;
        break;
    }

    while (online)
    {
        string opt;
        cin >> opt;
        int i = 0;
        for (; i < options.size(); i++)
        {
            if (opt == options[i].first)
            {
                (this->*(options[i].second))();
                break;
            }
        }
        if (i == options.size())
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

    AskAndSend("username");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("password");
    ComState opt = ComState(cli->Receive()[0]);
    switch (opt)
    {
    case ComState::ADMINISTRATOR:
        role = Role::Administrator;
        AddOptions_A();
        break;

    case ComState::TEACHER:
        role = Role::Teacher;
        AddOptions_T();
        break;

    case ComState::STUDENT:
        role = Role::Student;
        AddOptions_S();
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

    online = false;
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