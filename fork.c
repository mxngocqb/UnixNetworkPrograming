#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// #define DEBUGGING
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;

void sighandler(int signum)
{
    if (signum == SIGTERM)
    {
        FILE *f = fopen("clients.dat", "rb");
        char *buffer = "A client has send something \n";
        while (!feof(f))
        {
            int tmp = 0;
            fread(&tmp, sizeof(int), 1, f);
            printf("Sending to %d: \n", tmp);
            send(tmp, buffer, strlen(buffer), 0);
        }
        fclose(f);
        printf("Terminating...\n");
        exit(0);
    }
}

int main()
{
    int parentid = getpid();
    // signal(SIGINT, sighandler);
    // int *g_clients = NULL;
    // int count = 0;
    SOCKADDR_IN saddr, caddr;
    int len = sizeof(caddr);
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = 0;
    bind(s, (SOCKADDR *)&saddr, sizeof(saddr));
    listen(s, 10);

    while (0 == 0)
    {

        int c = accept(s, (SOCKADDR *)&caddr, (socklen_t *)&len);
        FILE *f = fopen("clients.dat", "ab");

        fwrite(&c, sizeof(int), 1, f);
        fclose(f);
        // g_clients = (int *)realloc(g_clients, (count + 1) * sizeof(int));
        // g_clients[count] = c;
        // count += 1;

        if (fork() == 0) // ở đây cũng thực hiện nhân đôi tiến trình
        {
            while (0 == 0)
            {
                // child process code
                // parent process don't implement this code
                char buffer[1024] = {0};
                int r = recv(c, buffer, sizeof(buffer) - 1, 0);
                if (r > 0)
                {
                    printf("Received %d bytes: %s from %d\n", r, buffer, c);
                    kill(parentid, SIGTERM);
                }
                else
                {
                    break;
                }
                // for (int i = 0; i < count; i++)
                // {
                //     send(g_clients[i], buffer, strlen(buffer), 0);
                // }
                // FILE *f = fopen("clients.dat", "rb");
                // while (!feof(f))
                // {
                //     int tmp = 0;
                //     fread(&tmp, sizeof(int), 1, f);
                //     printf("Sending to %d: \n", tmp);
                //     send(tmp, buffer, strlen(buffer), 0);
                // }
                // fclose(f);
            }
        }
    }
}