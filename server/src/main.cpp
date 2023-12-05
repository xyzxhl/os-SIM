#include "../include/Server.h"
#include <iostream>

using namespace std;

int main()
{

    Server server(12345);
    server.RunServer();
    cout << "Press Enter to stop." << endl;
    cin.get();

    server.StopServer();
}