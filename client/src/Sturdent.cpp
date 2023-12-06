#include "../include/SIM.h"
#include <iostream>

using namespace std;

void SIM::AddOptions_S()
{
    options.push_back(make_pair("PrintAssignmentTitle", &SIM::PrintAssignmentTitle));
    options.push_back(make_pair("PrintTitle", &SIM::DeleteUser));
    options.push_back(make_pair("SubmitHomework", &SIM::SubmitHomework));
    options.push_back(make_pair("PrintScore", &SIM::PrintScore));
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

    cli->Send(ComState::TASK_END);
}

void SIM::SubmitHomework()
{
    cli->Send(ComState::SUBMIT_HOMEWORK);

    cli->Send(ComState::TASK_END);
}

void SIM::PrintScore()
{
    cli->Send(ComState::PRINT_SCORE);

    cli->Send(ComState::TASK_END);
}