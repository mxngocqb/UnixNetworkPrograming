#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        short port = (short)atoi(argv[1]);
        // short port = 8888;
        int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        SOCKADDR_IN saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = 0;
        if (bind(s, (SOCKADDR *)&saddr, sizeof(SOCKADDR)) == 0)
        {
            listen(s, 10);

            SOCKADDR_IN caddr;

            int clen = sizeof(SOCKADDR_IN);
            // SOcket c
            int c = accept(s, (SOCKADDR *)&caddr, (socklen_t *)&clen);

            const char *msg = "send any comman to excute";

            send(c, msg, strlen(msg), 0);

            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            recv(c, buffer, sizeof(buffer) - 1, 0);
            if (buffer[strlen(buffer) - 1] == '\n')
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            sprintf(buffer + strlen(buffer), " > command.txt");
            printf("%s", buffer);
            system(buffer);
            FILE *f = fopen("command.txt", "rb");
            fseek(f, 0, SEEK_END);
            int size = ftell(f);
            char *data = (char *)calloc(size, 1);
            fseek(f, 0, SEEK_SET);
            fread(data, 1, size, f);
            fclose(f);
            int sent = 0;
            int tmp = 0;
            do
            {
                tmp = send(c, data, size, 0);
                sent += tmp;
            } while (sent < size && tmp >= 0);
            free(data);
            data = NULL;
            // system("ifconfig > command.txt")
        }
        else
        {
            printf("Failed to bind\n");
        }
    }
    else
    {
        printf("Parameter missing\n");
    }
} 