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
    cout << "Input \"Help\" to get help, or input other valid command. Case sensitive." << endl;
    while (true)
    {
        string command;
        cin >> command;
        if (command == "Help")
        {
            cout << endl
                 << "Help   -- get help\n"
                 << "Exit   -- exit system\n"
                 << "SIM    -- enter into \"Student Infomation Management\" system\n"
                 << "Create -- create a directory or file. Provide the path along with the name of the file you try to create.\n"
                 << "Delete -- delete the specified directory or file\n"
                 << "List   -- print all directory or file names in specified directory\n"
                 << "Read   -- read the specified file\n"
                 << "Write  -- write to he specified file\n"
                 << endl;
        }
        else if (command == "Exit")
        {
            break;
        }
        else if (command == "SIM")
        {
            sim.Interact();
            cout << "You are Back on the file system." << endl;
        }
        else if (command == "Create")
        {
            cout << "Which type do you want to create? (0:Directory, 1:File)" << endl;
            int isFile;
            cin >> isFile;
            if (isFile != 0 && isFile != 1)
                INVALID
            cout << "Input path:" << endl;
            string path;
            cin >> path;
            if (isFile)
                fileSys.CreateFile(path);
            else
                fileSys.CreateDirectory(path);
            cout << "Complete." << endl;
        }
        else if (command == "Delete")
        {
            cout << "Input path:" << endl;
            string path;
            cin >> path;
            fileSys.Delete(path);
            cout << "Complete." << endl;
        }
        else if (command == "List")
        {
            cout << "Input path:";
            string path;
            cin >> path;
            vector<string> list = fileSys.List(path);
            cout << "There are directories and files:" << endl;
            for (int i = 0; i < list.size(); i++)
            {
                cout << list[i] << endl;
            }
            cout << "\nComplete." << endl;
        }
        else if (command == "Read")
        {
            cout << "Input path:" << endl;
            string path;
            cin >> path;
            cout << "File content is:\n"
                 << fileSys.ReadFile(path) << endl;
            cout << "\nComplete." << endl;
        }
        else if (command == "Write")
        {
            cout << "Input path:" << endl;
            string path;
            cin >> path;
            cout << "Input content:" << endl;
            string content;
            cin >> content;
            fileSys.WriteFile(path, content);
            cout << "Complete." << endl;
        }
        else
            INVALID
    }
}