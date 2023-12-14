#include "../include/SIM.h"
#include "iostream"

using namespace std;

void SIM::AskAndSend(string q)
{
    string ss;
    cout << q << ":";
    cin >> ss;
    cli->Send(ss);
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

void SIM::ResetOptions()
{
    options.clear();
    ADD_OPTION(Exit)
    ADD_OPTION(PrintCourse)
    ADD_OPTION(PrintMember)
    options.push_back(make_pair("", nullptr));
}

void SIM::AddOptions_A()
{
    AddOptions_S();
    AddOptions_T();

    ADD_OPTION(AddUser)
    ADD_OPTION(DeleteUser)
    ADD_OPTION(AddCourse)
    ADD_OPTION(DeleteCourse)
    ADD_OPTION(Backup)
    ADD_OPTION(Recovery)
}

void SIM::AddOptions_S()
{
    ADD_OPTION(PrintAssignmentTitle)
    ADD_OPTION(PrintAssignmentContent)
    ADD_OPTION(SubmitHomework)
    ADD_OPTION(PrintScore)
    options.push_back(make_pair("", nullptr));
}

void SIM::AddOptions_T()
{
    ADD_OPTION(ReleaseAssignment)
    ADD_OPTION(PrintSubmittedHomework)
    ADD_OPTION(ReceiveHomework)
    ADD_OPTION(MarkHomework)
    options.push_back(make_pair("", nullptr));
}