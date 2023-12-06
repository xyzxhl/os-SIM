#pragma once

#include "../../common/include/FileSystem.h"
#include "../../common/include/ComState.h"

/*
File Structure:
(root)
|
|---User
|   |-username1
|   |   └-password(role is in this file)
|   |-username2
|   |   └-password
|   ...
|
└---Course
    |-course_name1
    |   |-assignment1
    |   |   |-Content
    |   |   └-Homework
    |   |       |-title1
    |   |       |   |-Score
    |   |       |   └-Content
    |   |       |-title2
    |   |       |   |-Score
    |   |       |   └-Content
    |   |       ...
    |   |
    |   |-assignment2
    |   |   └-...
    |   ...
    |
    |-course_name2
    |   └-...
    ...

*/
class Database
{
public:
    Database();

    ComState CheckLogin(std::string username, std::string password);

    std::string PrintCourse();
    std::string PrintMember();

    void AddUser(std::string username, std::string password, ComState role);
    void DeleteUser(std::string username);

    void AddCourse(std::string courseName);
    void DeleteCourse(std::string courseName);

    void ReleaseAssignment(std::string courseName, std::string assignment, std::string content);
    std::string PrintSubmittedHomework(std::string courseName, std::string assignment);
    std::string ReceiveHomework(std::string courseName, std::string assignment, std::string title);
    void MarkHomework(std::string courseName, std::string assignment, std::string title, std::string score);

    std::string PrintAssignmentTitle(std::string courseName);
    std::string PrintAssignmentContent(std::string courseName, std::string assignment);
    void SubmitHomework(std::string courseName, std::string assignment, std::string title, std::string content);
    std::string PrintScore(std::string courseName, std::string assignment, std::string title);

    void Backup();
    void Recovery();

private:
    FileSystem fileSys;
};