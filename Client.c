#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUMBER 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUMBER);
    servaddr.sin_addr.s_addr =  inet_addr("127.0.0.1");

    while (1) {
        // Send data to server
        // const char *message = "Hello from client!";
        // sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM,
        //        (const struct sockaddr *)&servaddr, sizeof(servaddr));

        // Receive data from server
        socklen_t len = sizeof(servaddr);
        int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                         (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        printf("Received message: %s\n", buffer);

        sleep(1);
    }

    // Close socket
    close(sockfd);
    return 0;
}
