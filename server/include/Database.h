#pragma once

#include "../../common/include/FileSystem.h"
#include "../../common/include/ComState.h"

class Database
{
public:
    Database();

    ComState CheckLogin(std::string username, std::string password);

    vector<std::string> GetCourse();
    vector<std::string> GetMember();

    void AddUser(std::string username, std::string password, ComState role);
    void DeleteUser(std::string username);

    void AddCourse(std::string courseName);
    void DeleteCourse(std::string courseName);

private:
    FileSystem fileSys;
};