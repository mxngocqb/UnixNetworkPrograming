#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUMBER 8080
#define MAXLINE 1024

int main()
{
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT_NUMBER);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Listening on port %d...\n", PORT_NUMBER);

    while (1)
    {
        // Receive data from client
        // socklen_t len = sizeof(cliaddr);
        // int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
        //                  (struct sockaddr *)&cliaddr, &len);
        // buffer[n] = '\0';
        // printf("Received message: %s\n", buffer);

        // Send data back to client
        const char *message = "Hello from server!";
        sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM,
               (const struct sockaddr *)&cliaddr, len);
    }

    // Close socket
    close(sockfd);
    return 0;
}
