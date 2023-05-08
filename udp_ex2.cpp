#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

// #define DEBUGGING
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
char msg[1024];

int main()
{
    const int broadcast_on = 1;
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast_on, sizeof(broadcast_on));
    SOCKADDR_IN toaddr, myaddr;

    toaddr.sin_family = AF_INET;
    toaddr.sin_port = htons(5000);
    toaddr.sin_addr.s_addr = inet_addr("192.168.56.255");

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(6000);
    myaddr.sin_addr.s_addr = 0;
    bind(s, (SOCKADDR *)&myaddr, sizeof(SOCKADDR));

    while (true)
    {
        printf("Input a message to send: ");
        memset(msg, 0, sizeof(msg));
        // input msg
        fgets(msg, sizeof(msg) - 1, stdin);
        // sendto
        sendto(s, msg, strlen(msg), 0, (SOCKADDR *)&toaddr, sizeof(SOCKADDR_IN));
        // recive
        memset(msg, 0, sizeof(msg));
        SOCKADDR_IN sender; // ip ben gui
        socklen_t slen = sizeof(SOCKADDR);
        int rev = recvfrom(s, msg, sizeof(msg) - 1, 0, (SOCKADDR *)&sender, &slen);
        printf("Received %d byte from: %s: %s\n", rev, inet_ntoa(sender.sin_addr), msg);
    }
}