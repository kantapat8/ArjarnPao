#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT1 12585
#define PORT2 22585
#define BUFFER_SIZE 1024

int main() {
    int clientSock1, clientSock2;
    struct sockaddr_in serverAddr1, serverAddr2;
    char buffer[BUFFER_SIZE];
    int bytesSent, bytesRecieved;

    // Create sockets
    clientSock1 = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock1 == -1) {
        perror("socket() error.");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr1, 0, sizeof(serverAddr1));
    serverAddr1.sin_family = AF_INET;
    serverAddr1.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr1.sin_port = htons(PORT1);

    clientSock2 = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock2 == -1) {
        perror("socket() error.");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr2, 0, sizeof(serverAddr2));
    serverAddr2.sin_family = AF_INET;
    serverAddr2.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr2.sin_port = htons(PORT2);

    // Connect to server
    if (connect(clientSock1, (struct sockaddr*)&serverAddr1, sizeof(serverAddr1)) == -1) {
        perror("Error connecting server 1.");
        exit(EXIT_FAILURE);
    }
    if (connect(clientSock2, (struct sockaddr*)&serverAddr2, sizeof(serverAddr2)) == -1) {
        perror("Error connecting server 2.");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server\n");

    // Get name and surname from user
    char name_surname[BUFFER_SIZE];
    printf("Enter your name and surname: ");
    fgets(name_surname, BUFFER_SIZE, stdin);
    name_surname[strcspn(name_surname, "\n")] = 0;

    // Send name and surname to server
    bytesSent = send(clientSock1, name_surname, strlen(name_surname), 0);
    if (bytesSent == -1) {
        perror("Error sending.");
        exit(EXIT_FAILURE);
    }
    printf("Name and surname sent to server\n");

    // Get birthdate from user
    char birthdate[BUFFER_SIZE];
    printf("Enter your birthdate (DDMMYYYY): ");
    fgets(birthdate, BUFFER_SIZE, stdin);
    birthdate[strcspn(birthdate, "\n")] = 0;

    // Send birthdate to server
    bytesSent = send(clientSock1, birthdate, strlen(birthdate), 0);
    if (bytesSent == -1) {
        perror("Error sending.");
        exit(EXIT_FAILURE);
    }
    printf("Birthdate sent to server\n");

    bytesRecieved = recv(clientSock1, buffer, BUFFER_SIZE, 0);
    if (bytesRecieved == -1) {
        perror("Error receiving.");
        exit(EXIT_FAILURE);
    }
    buffer[bytesRecieved] = '\0';  // Null-terminate the string
    printf("Server says: %s\n", buffer);

    // Receive CE year
    bytesRecieved = recv(clientSock2, buffer, BUFFER_SIZE, 0);
    if (bytesRecieved == -1) {
        perror("Error receiving CE year.");
        exit(EXIT_FAILURE);
    }
    buffer[bytesRecieved] = '\0';  // Null-terminate the string
    printf("Received CE year from port %d: %s\n", PORT2, buffer);

    // Send "Bye"
    bytesSent = send(clientSock2, "Bye", strlen("Bye"), 0);
    if (bytesSent == -1) {
        perror("Error sending \"Bye\".");
        exit(EXIT_FAILURE);
    }
    printf("Sent \"Bye\" to server port %d\n", PORT2);

    // Close sockets
    close(clientSock1);
    close(clientSock2);

    return 0;
}

