#include "../include/Database.h"

using namespace std;

Database::Database()
{
    fileSys.CreateDirectory("User");
    fileSys.CreateDirectory("Course");
}

void Database::AddUser(string username, string password, ComState role)
{
    string path = "User/" + username;
    fileSys.CreateDirectory(path);

    path = path + "/" + password;
    fileSys.CreateFile(path);

    fileSys.WriteFile(path, string(1, char(role)));
}

void Database::DeleteUser(string username)
{
    string path = "User/" + username;
    fileSys.Delete(path);
}

void Database::AddCourse(string courseName)
{
    string path = "Course/" + courseName;
    fileSys.CreateDirectory(path);
}

void Database::DeleteCourse(string courseName)
{
    string path = "Course/" + courseName;
    fileSys.Delete(path);
}

ComState Database::CheckLogin(std::string username, std::string password)
{
    string role = fileSys.ReadFile("User/" + username + "/" + password);

    if (role == "")
        return ComState::ERROR;

    return ComState(role[0]);
}