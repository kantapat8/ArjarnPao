#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT1 12585
#define PORT2 22585
#define BUFFER_SIZE 1024

int main() {
    int serverSock1, serverSock2;
    struct sockaddr_in serverAddr1, serverAddr2, clientAddr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];
    int bytesRecieved, bytesSent;

    // Create first socket
    serverSock1 = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock1 == -1) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr1, 0, sizeof(serverAddr1));
    serverAddr1.sin_family = AF_INET;
    serverAddr1.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr1.sin_port = htons(PORT1);

    // Create second socket
    serverSock2 = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock2 == -1) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&serverAddr2, 0, sizeof(serverAddr2));
    serverAddr2.sin_family = AF_INET;
    serverAddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr2.sin_port = htons(PORT2);

    // Bind sockets
    if (bind(serverSock1, (struct sockaddr*)&serverAddr1, sizeof(serverAddr1)) == -1 ) {
        perror("Error binding sock1");
        exit(EXIT_FAILURE);
    }

    if (bind(serverSock2, (struct sockaddr*)&serverAddr2, sizeof(serverAddr2)) == -1 ) {
        perror("Error binding sock2");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSock1, 3) == -1) {
        perror("Error listening sock1");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSock2, 3) == -1) {
        perror("Error listening sock2");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d and %d\n", PORT1, PORT2);

    // Accept connections
    int clientSock1;
    addr_len = sizeof(clientAddr);
    clientSock1 = accept(serverSock1, (struct sockaddr*)&clientAddr, &addr_len);
    if (clientSock1 == -1) {
        perror("Error accepting connection sock1");
        exit(EXIT_FAILURE);
    }

    int clientSock2;
    addr_len = sizeof(clientAddr);
    clientSock2 = accept(serverSock2, (struct sockaddr*)&clientAddr, &addr_len);
    if (clientSock2 == -1) {
        perror("Error accepting connection sock2");
        exit(EXIT_FAILURE);
    }

    printf("Client connected to port %d\n", PORT1);

    // Get name and surname
    bytesRecieved = recv(clientSock1, buffer, BUFFER_SIZE, 0);
    if (bytesRecieved == -1) {
        perror("Error receiving name and surname.");
        exit(EXIT_FAILURE);
    }
    buffer[bytesRecieved] = '\0';  // Null-terminate the string
    printf("Received name and surname from client: %s\n", buffer);

    // Get birthdate
    bytesRecieved = recv(clientSock1, buffer, BUFFER_SIZE, 0);
    if (bytesRecieved == -1) {
        perror("Error receiving birthdate.");
        exit(EXIT_FAILURE);
    }
    buffer[bytesRecieved] = '\0';  // Null-terminate the string

    // Change era
    int year = atoi(buffer + 4);
    year -= 543;
    printf("Received birthdate from client: %s\n", buffer);

    // Send confirmation to client
    bytesSent = send(clientSock1, "Server received", strlen("Server received"), 0);
    if (bytesSent == -1) {
        perror("Error sending confirmation.");
        exit(EXIT_FAILURE);
    }

    // Convert year to string and send back
    sprintf(buffer, "%d", year);
    bytesSent = send(clientSock2, buffer, strlen(buffer), 0);
    if (bytesSent == -1) {
        perror("Error sending year.");
        exit(EXIT_FAILURE);
    }
    printf("Year sent to client port %d\n", PORT2);

    // Bye
    bytesRecieved = recv(clientSock2, buffer, BUFFER_SIZE, 0);
    if (bytesRecieved == -1) {
        perror("Error receiving \"Bye\".");
        exit(EXIT_FAILURE);
    }

    buffer[bytesRecieved] = '\0';  // Null-terminate the string
    printf("Received from client port %d: %s\n", PORT2, buffer);

    close(clientSock1);
    close(clientSock2);
    close(serverSock1);
    close(serverSock2);

    return 0;
}
