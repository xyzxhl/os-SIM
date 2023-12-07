#include "../include/FileSystemUI.h"

int main()
{
    Client client(12345);
    FileSystemUI ui(&client);
    ui.Start();
    return 0;
}