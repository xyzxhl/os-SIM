#include "../include/Client.h"
#include "../include/Role.h"
#include "../../common/include/FileSystem.h"
#include <iostream>

using namespace std;

int main()
{
    Client client(12345);
    FileSystem fileSystem;
    BaseRole SMS(&fileSystem, &client);
    Role role = SMS.Login("admin", "123456");

    switch (role)
    {
    case Role::Administrator:
    {
        cout << "Successfully Login in. Welcome, administrator." << endl;
        Administrator *admin = reinterpret_cast<Administrator *>(&SMS);
        admin->Exit();
    }
    break;

    default:
        cerr << "Fail to login" << endl;
        break;
    }

    client.Disconnect();
    return 0;
}