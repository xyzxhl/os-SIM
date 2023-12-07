#include "../include/FileSystemUI.h"
#include <iostream>

#define INVALID                          \
    {                                    \
        cout << "Invalid input" << endl; \
        continue;                        \
    }

using namespace std;

FileSystemUI::FileSystemUI(Client *client)
    : cli(client),
      fileSys(),
      sim(&fileSys, client)
{
}

void FileSystemUI::Start()
{
    cout << "Input \"Help\" to get help, or input other valid command. Case sensitive.";
    while (true)
    {
        string command;
        cin >> command;
        if (command == "Help")
        {
            cout << "";
        }
        else if (command == "SIM")
        {
            sim.Interact();
        }
        else if (command == "Create")
        {
            cout << "Which type do you want to create? (0:Directory, 1:File)" << endl;
            int isFile;
            cin >> isFile;
            if (isFile != 0 || isFile != 1)
                INVALID
            cout << "Input path:";
            string path;
            cin >> path;
            if (isFile)
                fileSys.CreateFile(path);
            else
                fileSys.CreateDirectory(path);
        }
        else if (command == "Delete")
        {
            cout << "Input path:";
            string path;
            cin >> path;
            fileSys.Delete(path);
        }
        else if (command == "List")
        {
            cout << "Input path:";
            string path;
            cin >> path;
            vector<string> list = fileSys.List(path);
            for (int i = 0; i < list.size(); i++)
            {
                cout << list[i] << endl;
            }
        }
        else if (command == "Read")
        {
            cout << "Input path:";
            string path;
            cin >> path;
            cout << "File content is:\n"
                 << fileSys.ReadFile(path) << endl;
        }
        else if (command == "Write")
        {
            cout << "Input path:";
            string path;
            cin >> path;
            cout << "Input content:";
            string content;
            cin >> content;
            fileSys.WriteFile(path, content);
        }
    }
}