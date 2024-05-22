#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h>

#define PORT 8080 
#define BUFFER_SIZE 1024

int main() { int sock = 0; struct sockaddr_in serv_addr; char *name = "Enter your name: "; char buffer[BUFFER_SIZE] = {0};  
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("%s", name);
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

    // Send name to server
    send(sock, buffer, strlen(buffer), 0);

    // Receive greeting message from server
    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}