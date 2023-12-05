#pragma once

#include "../../common/include/FileSystem.h"
#include "Client.h"
#include <string>
#include <vector>

enum class Role
{
    Unknown,
    Administrator,
    Teacher,
    Student
};

class BaseRole
{
public:
    BaseRole(FileSystem *fileSystem, Client *client);

protected:
    void SendError() { cli->Send(ComState::ERROR); }
    void SendEnd() { cli->Send(ComState::TASK_END); }

public:
    Role Login(std::string username, std::string password);
    void Interact();
    void Exit();

    void PrintCourse();
    void PrintMember();

protected:
    FileSystem *fileSys;
    Client *cli;
    bool online;
    std::vector<std::string> options;
};

/*
                                |   Filename    |   Content
    Assignment(teachers upload) |   title       |   what stu should do
    Homework(students upload)   |   stu name    |   homework content
*/

class Teacher : virtual public BaseRole
{
public:
    void AssignHomework(std::string courseName, std::string title, std::string path);
    void PrintReceivedHomework(std::string courseName, std::string title);
    void ReceiveHomework(std::string courseName, std::string title);
    void MarkHomework(std::string courseName, std::string title, std::string studentName, int score);

    void Interact();
};

class Student : virtual public BaseRole
{
public:
    void PrintTitle(std::string courseName);
    void GetHomework(std::string courseName, std::string title);
    void SubmitHomework(std::string courseName, std::string title, std::string path);
    void GetScore(std::string courseName, std::string title);

    void Interact();
};

class Administrator : public Teacher, public Student
{
public:
    void AddUser(std::string username, std::string password, Role role);
    void DeleteUser(std::string username);

    void AddCourse(std::string courseName);
    void DeleteCourse(std::string courseName);

    void Backup();
    void Recovery();

    void Interact();
};