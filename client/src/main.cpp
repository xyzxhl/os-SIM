#include "../include/SIM.h"

int main()
{
    Client client(12345);
    FileSystem fileSystem;
    SIM SMS(&fileSystem, &client);
    SMS.Interact();
    return 0;
}