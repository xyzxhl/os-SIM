#include "../include/Database.h"
#include <vector>

using namespace std;

Database::Database()
{
    fileSys.CreateDirectory("User");
    fileSys.CreateDirectory("Course");
}

ComState Database::CheckLogin(string username, string password)
{
    lock_guard<mutex> lock(userMutex);
    string role = fileSys.ReadFile("User/" + username + "/" + password);

    if (role == "")
        return ComState::ERROR;

    return ComState(role[0]);
}

string Database::PrintCourse()
{
    lock_guard<mutex> lock(courseMutex);
    vector<string> list = fileSys.List("Course");
    if (list.size() == 0)
    {
        return "There is no course.\n";
    }

    string res = "There are these courses:\n";
    for (int i = 0; i < list.size(); i++)
    {
        res = res + list[i] + "\n";
    }
    return res;
}

string Database::PrintMember()
{
    lock_guard<mutex> lock(userMutex);
    vector<string> list = fileSys.List("User");
    if (list.size() == 0)
    {
        return "There is no member.\n";
    }

    string res = "There are these members:\n";
    for (int i = 0; i < list.size(); i++)
    {
        res = res + list[i] + "\n";
    }
    return res;
}

void Database::AddUser(string username, string password, ComState role)
{
    lock_guard<mutex> lock(userMutex);
    string path = "User/" + username;
    fileSys.CreateDirectory(path);

    path = path + "/" + password;
    fileSys.CreateFile(path);

    fileSys.WriteFile(path, string(1, char(role)));
}

void Database::DeleteUser(string username)
{
    lock_guard<mutex> lock(userMutex);
    string path = "User/" + username;
    fileSys.Delete(path);
}

void Database::AddCourse(string courseName)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName;
    fileSys.CreateDirectory(path);
}

void Database::DeleteCourse(string courseName)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName;
    fileSys.Delete(path);
}

void Database::ReleaseAssignment(string courseName, string assignment, string content)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment;
    fileSys.CreateDirectory(path);
    fileSys.CreateDirectory(path + "/Homework");
    fileSys.CreateFile(path + "/Content");
    fileSys.WriteFile(path + "/Content", content);
}

string Database::PrintSubmittedHomework(string courseName, string assignment)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment + "/Homework";
    vector<string> list = fileSys.List(path);
    if (list.size() == 0)
    {
        return "There is no submitted homework.\n";
    }

    string res = "There are these submitted homework:\n";
    for (int i = 0; i < list.size(); i++)
    {
        res = res + list[i] + "\n";
    }
    return res;
}

string Database::ReceiveHomework(string courseName, string assignment, string title)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment + "/Homework/" + title + "/Content";
    return fileSys.ReadFile(path);
}

void Database::MarkHomework(string courseName, string assignment, string title, string score)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment + "/Homework/" + title + "/Score";
    fileSys.WriteFile(path, score);
}

string Database::PrintAssignmentTitle(string courseName)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName;
    vector<string> list = fileSys.List(path);
    if (list.size() == 0)
    {
        return "There is no assignment.\n";
    }

    string res = "There are these assignments:\n";
    for (int i = 0; i < list.size(); i++)
    {
        res = res + list[i] + "\n";
    }
    return res;
}

string Database::PrintAssignmentContent(string courseName, string assignment)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment + "/Content";
    return fileSys.ReadFile(path);
}

void Database::SubmitHomework(string courseName, string assignment, string title, string content)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment + "/Homework/" + title;
    fileSys.CreateDirectory(path);
    fileSys.CreateFile(path + "/Score");
    fileSys.CreateFile(path + "/Content");
    fileSys.WriteFile(path + "/Content", content);
}

string Database::PrintScore(string courseName, string assignment, string title)
{
    lock_guard<mutex> lock(courseMutex);
    string path = "Course/" + courseName + "/" + assignment + "/Homework/" + title + "/Score";
    return fileSys.ReadFile(path);
}

void Database::Backup()
{
    lock_guard<mutex> lock(userMutex);
    lock_guard<mutex> lock(courseMutex);
    fileSys.Delete("Backup");
    fileSys.CreateDirectory("Backup");
    fileSys.Copy("User", "Backup");
    fileSys.Copy("Course", "Backup");
}

void Database::Recovery()
{
    lock_guard<mutex> lock(userMutex);
    lock_guard<mutex> lock(courseMutex);
    fileSys.Delete("User");
    fileSys.Delete("Course");
    fileSys.Copy("Backup/User", "");
    fileSys.Copy("Backup/Course", "");
}