#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
int main()
{
    if (fork() == 0)
    { // Process 1
        int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        SOCKADDR_IN myaddr, sender;
        int clen = sizeof(sender);
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons(8888);
        myaddr.sin_addr.s_addr = 0;
        bind(s, (SOCKADDR *)&myaddr, sizeof(myaddr));
        char buffer[1024];
        while (0 == 0)
        {
            memset(buffer, 0, sizeof(buffer));
            recvfrom(s, buffer, sizeof(buffer) - 1, 0, (SOCKADDR *)&sender, (socklen_t *)&clen);
            if (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r')
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            FILE *f = fopen("clients.txt", "at");
            fprintf(f, "%s %s\n", buffer, inet_ntoa(sender.sin_addr));
            fclose(f);
        }
    }
    else
    {
        if (fork() == 0)
        { // Process 2
            int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            SOCKADDR_IN myaddr, client;
            int clen = sizeof(client);
            myaddr.sin_family = AF_INET;
            myaddr.sin_port = htons(5000);
            myaddr.sin_addr.s_addr = 0;
            bind(s, (SOCKADDR *)&myaddr, sizeof(myaddr));
            listen(s, 10);
            while (0 == 0)
            {
                int c = accept(s, (SOCKADDR *)&client, (socklen_t *)&clen);
                FILE *f = fopen("clients.txt", "rb");
                fseek(f, 0, SEEK_END);
                int size = ftell(f);
                fseek(f, 0, SEEK_SET);
                char *data = (char *)calloc(size + 1, 1);
                fread(data, 1, size, f);
                fclose(f);
                int sent = 0;
                while (sent < size)
                {
                    int tmp = send(c, data + sent, size - sent, 0);
                    if (tmp > 0)
                    {
                        sent += tmp;
                    }
                    else
                        break;
                }
                close(c);
                free(data);
                data = NULL;
            }
        }
    }
    printf("Press anykey to exit...");
    getchar();
}