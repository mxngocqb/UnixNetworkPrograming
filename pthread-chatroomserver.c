#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
int *g_client = NULL;
int g_count = 0;

void *ClientThread(void *arg)
{
    int c = *(int *)arg;
    free(arg);
    arg = NULL;
    char buffer[1024];
    char buffer2[1024];
    while (0 == 0)
    {
        memset(buffer, 0, sizeof(buffer));
        int r = recv(c, buffer, sizeof(buffer) - 1, 0);
        if (r > 0)
        {
            printf("Recv from (%d): %s\n", c, buffer);
            for (int i = 0; i < g_count; i++)
            {
                sprintf(buffer2, "From client %d: ", c);
                strcat(buffer2, buffer);
                if (g_client[i] != c)
                {
                    send(g_client[i], buffer2, strlen(buffer2), 0);
                }
            }
        }
        else
            break;
    }
    printf("%d disconnected\n", c);
    return NULL;
}
int main()
{
    SOCKADDR_IN myaddr, caddr;
    int clen = sizeof(caddr);
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    myaddr.sin_family = AF_INET;                  // IPv4
    myaddr.sin_port = htons(5000);                // Random port 50000
    myaddr.sin_addr.s_addr = 0;                   //
    bind(s, (SOCKADDR *)&myaddr, sizeof(myaddr)); // Sign addr and port to socket
    listen(s, 10);                                // listen
    while (0 == 0)
    {
        int c = accept(s, (SOCKADDR *)&caddr, (socklen_t *)&clen); //
        g_client = (int *)realloc(g_client, (g_count + 1) * sizeof(int));
        g_client[g_count] = c;
        g_count += 1;
        pthread_t tid;
        int *arg = (int *)calloc(1, sizeof(int));
        *arg = c;
        printf("Client %d: \n", c);
        pthread_create(&tid, NULL, ClientThread, (void *)arg); // create a new thread
    }
}