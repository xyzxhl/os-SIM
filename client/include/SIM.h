#pragma once

#include "../../common/include/FileSystem.h"
#include "Client.h"
#include <string>
#include <vector>
#include <utility>

#define SEND_ERROR_AND_END          \
    {                               \
        cli->Send(ComState::ERROR); \
        online = false;             \
        return;                     \
    }

#define ASK_AND_SEND(q)    \
    {                      \
        string ss;         \
        cout << #q << ":"; \
        cin >> ss;         \
        cli->Send(ss);     \
    }

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
    void AddOptions_T();

    void AssignHomework();
    void PrintReceivedHomework();
    void ReceiveHomework();
    void MarkHomework();

    // Student
    void AddOptions_S();

    void PrintTitle();
    void GetHomework();
    void SubmitHomework();
    void GetScore();

    // Administrator
    void AddOptions_A();

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
