#include "../include/Server.h"
#include "../include/Handler.h"
#include "../../common/include/ComState.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

using namespace std;

Server::Server(int port)
    : port(port),
      serverSocket(-1)
{
    database.AddUser("admin", "123456", ComState::ADMINISTRATOR);
}

Server::~Server()
{
    StopServer();
}

void Server::RunServer()
{
    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Failed to create socket." << endl;
        return;
    }

    // Bind the socket to an IP address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        cerr << "Failed to bind socket." << endl;
        close(serverSocket);
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        cerr << "Failed to listen on socket." << endl;
        close(serverSocket);
        return;
    }

    cout << "Server started. Listening on port " << port << endl;

    acceptingThread = thread(&Server::StartAccepting, this);
}

void Server::StopServer()
{
    if (serverSocket != -1)
    {
        close(serverSocket);
        serverSocket = -1;
    }

    if (acceptingThread.joinable())
    {
        acceptingThread.join();
    }
}

void Server::StartAccepting()
{
    // Set timeout to 1 seconds
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    while (serverSocket != -1)
    {
        // Create a file descriptor set and add the server socket
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);

        // Use select to monitor the socket with a timeout
        int ready = select(serverSocket + 1, &readSet, nullptr, nullptr, &timeout);
        if (ready == -1)
        {
            std::cout << "Error in select." << std::endl;
            return;
        }
        else if (ready == 0)
        {
            // Timeout occurred
            continue;
        }
        else
        {
            sockaddr_in clientAddress{};
            socklen_t clientAddressLength = sizeof(clientAddress);
            int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressLength);
            if (clientSocket == -1)
            {
                cerr << "Failed to accept connection." << endl;
                continue;
            }

            cout << "Client connected." << endl;

            thread clientThread(&Server::HandleClient, this, clientSocket);
            clientThread.detach();
        }
    }
}

void Server::HandleClient(int clientSocket)
{
    Handler handler(&database, this, clientSocket);
    handler.Handle();

    // Close the client socket
    close(clientSocket);
}

void Server::Send(string content, int clientSocket)
{
    const char *message = content.data();
    ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent == -1)
    {
        cerr << "Failed to send data to server." << endl;
    }
}

void Server::Send(ComState state, int clientSocket)
{
    Send(string(1, char(state)), clientSocket);
}

string Server::Receive(int clientSocket)
{
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1)
    {
        cerr << "Failed to receive data from client." << endl;
        return "";
    }
    buffer[bytesRead] = '\0';

    return string(buffer);
}
