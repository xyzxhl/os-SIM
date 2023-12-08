#include "../include/SIM.h"
#include <iostream>

using namespace std;

void SIM::AddOptions_T()
{
    ADD_OPTION(ReleaseAssignment)
    ADD_OPTION(PrintSubmittedHomework)
    ADD_OPTION(ReceiveHomework)
    ADD_OPTION(MarkHomework)
    options.push_back(make_pair("", nullptr));
}

void SIM::ReleaseAssignment()
{
    cli->Send(ComState::RELEASE_ASSIGNMENT);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("content");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}

void SIM::PrintSubmittedHomework()
{
    cli->Send(ComState::PRINT_SUBMITTED_HOMEWORK);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    cout << "Homework titles:\n"
         << cli->Receive();

    cli->Send(ComState::TASK_END);
}

void SIM::ReceiveHomework()
{
    cli->Send(ComState::RECEIVE_HOMEWORK);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    string title;
    cout << "title:";
    cin >> title;
    cli->Send(title);
    string content = cli->Receive();

    cli->Send(ComState::TASK_END);

    string path;
    cout << "target path:";
    cin >> path;
    path = path + "/" + title;
    fileSys->CreateFile(path);
    fileSys->WriteFile(path, content);
    cout << "Complete\n"
         << endl;
}

void SIM::MarkHomework()
{
    cli->Send(ComState::MARK_HOMEWORK);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("title");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("score");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
    cout << "Complete\n"
         << endl;
}
