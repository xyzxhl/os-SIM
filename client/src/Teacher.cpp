#include "../include/SIM.h"
#include <iostream>

using namespace std;

void SIM::AddOptions_T()
{
    options.push_back(make_pair("ReleaseAssignment", &SIM::ReleaseAssignment));
    options.push_back(make_pair("PrintSubmittedHomework", &SIM::PrintSubmittedHomework));
    options.push_back(make_pair("ReceiveHomework", &SIM::ReceiveHomework));
    options.push_back(make_pair("MarkHomework", &SIM::MarkHomework));
}

void SIM::ReleaseAssignment()
{
    cli->Send(ComState::RELEASE_ASSIGNMENT);

    cli->Send(ComState::TASK_END);
}

void SIM::PrintSubmittedHomework()
{
    cli->Send(ComState::PRINT_SUBMITTED_HOMEWORK);

    cli->Send(ComState::TASK_END);
}

void SIM::ReceiveHomework()
{
    cli->Send(ComState::RECEIVE_HOMEWORK);

    cli->Send(ComState::TASK_END);
}

void SIM::MarkHomework()
{
    cli->Send(ComState::MARK_HOMEWORK);

    cli->Send(ComState::TASK_END);
}
