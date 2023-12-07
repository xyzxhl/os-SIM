#include "../include/SIM.h"
#include <iostream>

using namespace std;

void SIM::AddOptions_S()
{
    ADD_OPTION(PrintAssignmentTitle)
    ADD_OPTION(PrintAssignmentContent)
    ADD_OPTION(SubmitHomework)
    ADD_OPTION(PrintScore)
}

void SIM::PrintAssignmentTitle()
{
    cli->Send(ComState::PRINT_ASSIGNMENT_TITLE);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    cout << cli->Receive();

    cli->Send(ComState::TASK_END);
}

void SIM::PrintAssignmentContent()
{
    cli->Send(ComState::PRINT_ASSIGNMENT_CONTENT);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    cout << cli->Receive() << endl;

    cli->Send(ComState::TASK_END);
}

void SIM::SubmitHomework()
{
    cli->Send(ComState::SUBMIT_HOMEWORK);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    string name, content, path;
    cout << "file path:";
    cin >> path;
    name = fileSys->GetFileName(path);
    content = fileSys->ReadFile(path);

    cli->Send(name);
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(content);
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    cli->Send(ComState::TASK_END);
}

void SIM::PrintScore()
{
    cli->Send(ComState::PRINT_SCORE);
    if (ComState(cli->Receive()[0]) != ComState::ACCEPT_REQ)
        SEND_ERROR_AND_END

    AskAndSend("course name");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("assignment");
    if (ComState(cli->Receive()[0]) != ComState::SUCCESS_RECV)
        SEND_ERROR_AND_END

    AskAndSend("title");
    cout << cli->Receive() << endl;

    cli->Send(ComState::TASK_END);
}