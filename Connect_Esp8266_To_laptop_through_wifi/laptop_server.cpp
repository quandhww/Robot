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

int main() {
    int serverSocket, clientHandler;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the port
    // if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    //     perror("setsockopt failed");
    //     exit(EXIT_FAILURE);
    // }

    // Define address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;

    // Accept a client connection
    if ((clientHandler = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Client connected!" << std::endl;

    // Read data from client
    int valread = read(clientHandler, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << std::endl;

    // Send response
    const char *message = "Hello from server";
    send(clientHandler, message, strlen(message), 0);
    std::cout << "Response sent" << std::endl;

    // Close sockets
    close(clientHandler);
    close(serverSocket);

    return 0;
}