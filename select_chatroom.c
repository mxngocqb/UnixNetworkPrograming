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

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
int *g_clients = NULL;
int *g_status = NULL;
int g_count = 0;
int main()
{
    SOCKADDR_IN myaddr, caddr;
    int clen = sizeof(caddr);
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(9999);
    myaddr.sin_addr.s_addr = 0; // because that is the server machine
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(s, (SOCKADDR *)&myaddr, sizeof(myaddr));
    listen(s, 10);
    fd_set set1; // data go into
    while (0 == 0)
    {
        FD_ZERO(&set1);   // clear all set 1, don't you any socket in that
        FD_SET(s, &set1); // add socket s into set 1
        for (int i = 0; i < g_count; i++)
        {
            if (g_status[i] == 1)
            {
                FD_SET(g_clients[i], &set1);
            }
        }
        select(FD_SETSIZE, &set1, NULL, NULL, NULL);
        // Accept socket connect to server
        if (FD_ISSET(s, &set1)) // find socket s in the set
        {
            int c = accept(s, (SOCKADDR *)&caddr, (socklen_t *)&clen);
            g_clients = (int *)realloc(g_clients, (g_count + 1) * sizeof(int));
            g_status = (int *)realloc(g_status, (g_count + 1) * sizeof(int));
            g_clients[g_count] = c;
            g_status[g_count] = 1;
            g_count += 1;
            printf("SOCKET %d connected\n", c);
        }

        for (int i = 0; i < g_count; i++)
        {
            // find the socket client in the set
            if (g_status[i] == 1 && FD_ISSET(g_clients[i], &set1))
            {
                char buffer[1024] = {0};
                int r = recv(g_clients[i], buffer, sizeof(buffer) - 1, 0);
                if (r > 0)
                {
                    printf("%d: %s\n", g_clients[i], buffer);
                    for (int k = 0; k < g_count; k++)
                    {
                        if (k != i && g_status[k] == 1)
                        {
                            send(g_clients[k], buffer, strlen(buffer), 0);
                        }
                    }
                }
                else
                {
                    g_status[i] = 0;
                }
            }
        }
    }
}