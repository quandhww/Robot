/*
To do:
Write all the program again by hand
*/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1369
#define BUFFER_SIZE 1024

int main()
{
    std::string staticServerEthernetIpAddress = "192.168.99.200";
    struct sockaddr_in socketServerAddress;
    socketServerAddress.sin_family = AF_INET;
    if(inet_pton(AF_INET, staticServerEthernetIpAddress.c_str(), &socketServerAddress.sin_addr) <= 0)
    {
        std::cerr << "Can not set ip address to socketServerAddress!";
        exit(EXIT_FAILURE);
    }
    socketServerAddress.sin_port = htons(PORT);
    /*If PROTOCOL is zero, one is chosen automatically*/
    int automaticProtocol = 0;
    int serverSocket = socket(AF_INET, SOCK_STREAM, automaticProtocol);
    if(serverSocket == -1)
    {
        std::cerr << "Can not create serverSocket!";
        exit(EXIT_FAILURE);
    }

    if(bind(serverSocket, 
        reinterpret_cast<struct sockaddr*>(&socketServerAddress), 
        sizeof(socketServerAddress)) != 0)
    {
        std::cerr << "Can not bind socketServerAddress to serverSocket!" << '\n';
        exit(EXIT_FAILURE);
    }

    if(listen(serverSocket, 5) != 0)
    {
        std::cerr << "Can not listen on serverSocket!" << '\n';
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on 192.168.99.200:" << PORT << '\n';

    socklen_t socketServerAddress_len = sizeof(socketServerAddress);
    int clientHandler = accept(serverSocket, 
        reinterpret_cast<struct sockaddr*>(&socketServerAddress),
        &socketServerAddress_len);
    if(clientHandler == -1)
    {
        std::cerr << "Can not accept connection from client!" << '\n';
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clientAddress;
    socklen_t clientAddress_len = sizeof(clientAddress);
    getpeername(clientHandler, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddress_len);
    char clientIpAdress[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clientAddress.sin_addr, clientIpAdress, INET_ADDRSTRLEN) == NULL) 
    {
        std::cout << "Can not convert ip address of client\n";
        exit(1);
    }
    std::cout << "Accept connection from client " << std::string(clientIpAdress)<< ":" << clientAddress.sin_port << '\n';


    char buffer[BUFFER_SIZE] = {0};
    if(read(clientHandler, buffer, BUFFER_SIZE) == -1)
    {
        std::cout << "Can not read client!" << '\n';
    }
    std::cout << "From Client: " << buffer << '\n';
    std::string msg = "This is msg from server!";
    int blockingMode = 0;
    if(send(clientHandler, msg.c_str(), msg.size(), blockingMode) == -1)
    {
        std::cout << "Can not send msg back to client!";
    }
    close(clientHandler);
    close(serverSocket);   
}