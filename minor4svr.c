/*
Emilio Pena
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PING_MSG_SIZE 512
#define LOSS_THRESHOLD 30 // 30% packet loss

void die(char *message) {
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]) {
    // Check for correct command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int server_socket, addr_len, lost;
    struct sockaddr_in server_addr, client_addr;
    char ping_msg[PING_MSG_SIZE];
    
    // Create UDP socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        die("socket");
    }

    // Set up server address structure
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1])); // Convert port number to network byte order
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        die("bind");
    }

    addr_len = sizeof(client_addr);

    printf("Server listening on port %s...\n", argv[1]);

    while (1) {
        // Receive PING message from client
        if (recvfrom(server_socket, ping_msg, PING_MSG_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len) == -1) {
            die("recvfrom");
        }

        // Simulate packet loss (30%)
        lost = rand() % 100;
        if (lost < LOSS_THRESHOLD) {
            printf("Packet from client %s:%d lost!\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        } else {
            // Print received message and send PONG response back to the client
            printf("Received message from client %s:%d: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), ping_msg);
            if (sendto(server_socket, "PONG", strlen("PONG"), 0, (struct sockaddr *)&client_addr, addr_len) == -1) {
                die("sendto");
            }
        }
    }

    // Close the socket
    close(server_socket);

    return 0;
}