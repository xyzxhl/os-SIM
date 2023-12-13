#pragma once

#include "../../common/include/FileSystem.h"
#include "Client.h"
#include <vector>
#include <utility>

#define SEND_ERROR_AND_END          \
    {                               \
        cli->Send(ComState::ERROR); \
        online = false;             \
        return;                     \
    }

#define ADD_OPTION(opt) options.push_back(make_pair(#opt, &SIM::opt));

enum class Role
{
    Unknown,
    Administrator,
    Teacher,
    Student
};

class SIM
{
public:
    SIM(FileSystem *fileSystem, Client *client);

    void Interact();

private:
    // Utility
    void AskAndSend(std::string);

    void AddOptions_T();
    void AddOptions_S();
    void AddOptions_A();

    void PrintOptions();

    void ResetOptions();

    // BaseRole
    void Login();
    void Exit();

    void PrintCourse();
    void PrintMember();

    /*
                                |   Filename    |   Content
    Assignment(teachers upload) |   title       |   what stu should do
    Homework(students upload)   |   stu name    |   homework content
    */

    // Teacher
    void ReleaseAssignment();
    void PrintSubmittedHomework();
    void ReceiveHomework();
    void MarkHomework();

    // Student
    void PrintAssignmentTitle();
    void PrintAssignmentContent();
    void SubmitHomework();
    void PrintScore();

    // Administrator
    void AddUser();
    void DeleteUser();
    void AddCourse();
    void DeleteCourse();
    void Backup();
    void Recovery();

private:
    FileSystem *fileSys;
    Client *cli;
    bool online;
    std::vector<std::pair<std::string, void (SIM::*)()>> options;

    Role role;
};
