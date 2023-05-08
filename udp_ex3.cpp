#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
char msg[1024];
SOCKADDR_IN *senders = NULL;
int count = 0;
int main()
{
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN myaddr, sender;
    int slen = sizeof(SOCKADDR_IN);
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(5000);
    myaddr.sin_addr.s_addr = 0;
    bind(s, (SOCKADDR *)&myaddr, sizeof(SOCKADDR_IN));

    while (0 == 0)
    {
        memset(msg, 0, sizeof(msg));
        int received = recvfrom(s, msg, sizeof(msg) - 1, 0, (SOCKADDR *)&sender, (socklen_t *)&slen);
        printf("Received %d bytes from %s: %s\n", received, inet_ntoa(sender.sin_addr), msg);
        senders = (SOCKADDR_IN *)realloc(senders, (count + 1) * sizeof(SOCKADDR_IN));
        memcpy(senders + count, &sender, sizeof(SOCKADDR_IN));
        count += 1;
        for (int i = 0; i < count; i++)
        {
            sendto(s, msg, strlen(msg), 0, (SOCKADDR *)&(senders[i]), sizeof(SOCKADDR_IN));
        }
    }
    close(s);
    free(senders);
    senders = NULL;
}