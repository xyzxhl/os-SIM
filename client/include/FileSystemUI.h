#pragma once

#include "SIM.h"
#include "Client.h"

class FileSystemUI
{
public:
    FileSystemUI(Client *client);

    void Start();

private:
    FileSystem fileSys;
    SIM sim;
    Client *cli;
};