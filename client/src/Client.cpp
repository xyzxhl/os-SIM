#include "../include/Client.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

Client::Client(int port)
    : port(port),
      clientSocket(-1)
{
}

Client::~Client()
{
    Disconnect();
}

void Client::Connect()
{
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Failed to create socket." << endl;
        return;
    }

    // Connect to the server
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port); // Port number
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        cerr << "Invalid address." << endl;
        close(clientSocket);
        return;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        cerr << "Failed to connect to server." << endl;
        close(clientSocket);
        return;
    }

    cout << "Connected to server." << endl;
}

void Client::Disconnect()
{
    if (clientSocket != -1)
    {
        close(clientSocket);
        clientSocket = -1;
    }
}

void Client::Send(string content)
{
    const char *message = content.data();
    ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent == -1)
    {
        cerr << "Failed to send data to server." << endl;
    }
}

void Client::Send(ComState state)
{
    Send(string(1, char(state)));
}

string Client::Receive()
{
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1)
    {
        cerr << "Failed to receive data from server." << endl;
        return "";
    }
    buffer[bytesRead] = '\0';

    return string(buffer);
}