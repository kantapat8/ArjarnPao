#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h>

#define PORT 8080 
#define BUFFER_SIZE 1024

int main() { int server_fd, new_socket; struct sockaddr_in address; int addrlen = sizeof(address); char buffer[BUFFER_SIZE] = {0}; char *greeting = "Hello ";
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the network address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read client's name
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Client's name: %s\n", buffer);

    // Send greeting message to client
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response), "%s%s", greeting, buffer);
    send(new_socket, response, strlen(response), 0);

    // Close the socket
    close(new_socket);
    close(server_fd);

    return 0;
}