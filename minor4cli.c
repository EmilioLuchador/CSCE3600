/*
Emilio Pena
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define PING_MSG_SIZE 512
#define NUM_PING_MESSAGES 10

void die(char *message) {
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]) {
    // Check for correct command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1);
    }

    int client_socket, addr_len, received_msgs = 0;
    struct sockaddr_in server_addr;
    char ping_msg[PING_MSG_SIZE];
    struct timeval start_time, end_time;
    double rtt_total = 0, rtt_min = 1000000, rtt_max = 0;

    // Create UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        die("socket");
    }

    // Set up server address structure
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], &server_addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address\n");
        exit(1);
    }

    addr_len = sizeof(server_addr);

    // Send 10 automated PING messages
    for (int i = 0; i < NUM_PING_MESSAGES; ++i) {
        // Build PING message
        snprintf(ping_msg, PING_MSG_SIZE, "PING %d", i);

        // Get current time before sending the message
        gettimeofday(&start_time, NULL);

        // Send the PING message
        if (sendto(client_socket, ping_msg, strlen(ping_msg), 0, (struct sockaddr *)&server_addr, addr_len) == -1) {
            die("sendto");
        }

        // Receive response or timeout
        if (recvfrom(client_socket, ping_msg, PING_MSG_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len) == -1) {
            printf("Request %d: Timed out\n", i);
        } else {
            // Get current time after receiving the response
            gettimeofday(&end_time, NULL);

            // Calculate round trip time (RTT) in milliseconds
            double rtt = (double)(end_time.tv_usec - start_time.tv_usec) / 1000.0;
            rtt += (double)(end_time.tv_sec - start_time.tv_sec) * 1000.0;

            // Update RTT statistics
            rtt_total += rtt;
            if (rtt < rtt_min) rtt_min = rtt;
            if (rtt > rtt_max) rtt_max = rtt;

            printf("Request %d: Received response in %.3f ms\n", i, rtt);
            received_msgs++;
        }

        // Wait for 1 second before sending the next message
        sleep(1);
    }

    // Calculate message loss rate
    double loss_rate = ((double)(NUM_PING_MESSAGES - received_msgs) / NUM_PING_MESSAGES) * 100.0;

    // Calculate average RTT
    double rtt_avg = rtt_total / received_msgs;

    // Print summary
    printf("\n--- Ping statistics ---\n");
    printf("%d messages sent, %d messages received, %.2f%% message loss\n", NUM_PING_MESSAGES, received_msgs, loss_rate);
    printf("RTT: min=%.3f ms, max=%.3f ms, avg=%.3f ms\n", rtt_min, rtt_max, rtt_avg);

    // Close the socket
    close(client_socket);

    return 0;
}
