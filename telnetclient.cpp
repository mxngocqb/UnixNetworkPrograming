#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

//#define DEBUGGING
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
//#define DEBUGGING
int main(int argc, char** argv)
{
#ifdef DEBUGGING
    argc = 3;
#endif
    if (argc < 3)
    {
        printf("Parameter missing\n");
    }else
    {
#ifdef DEBUGGING
    in_addr_t ip = inet_addr("127.0.0.1");
    short port = 8888;
#else
        in_addr_t ip = inet_addr(argv[1]);
        short port = (short)atoi(argv[2]);
#endif
        int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        SOCKADDR_IN saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = ip;
        int error = connect(s, (SOCKADDR*)&saddr, sizeof(SOCKADDR));
        char buffer[1024];

        //Receive welcome message from server
        memset(buffer, 0, sizeof(buffer));
        int received = 0;
        int tmp = 0;
        int bytes_to_received = 0;
        do
        {
            bytes_to_received = sizeof(buffer) - received - 1;
            tmp = recv(s, buffer + received, bytes_to_received, 0);
            received += tmp;
        } while ((tmp >= 0) && (tmp == bytes_to_received));
        printf("%s\n", buffer);

        while (error == 0)
        {
            //Receive command from stdin
            memset(buffer, 0, sizeof(buffer));
#ifdef DEBUGGING
            strcpy(buffer,"cat telnetclient.cpp\n");
#else
            fgets(buffer, sizeof(buffer) - 1, stdin);
#endif
            send(s, buffer, strlen(buffer), 0);
            //Receive reponse from server
            received = 0;
            char* response = NULL;
            do
            {
                memset(buffer, 0, sizeof(buffer));
                tmp = recv(s, buffer, sizeof(buffer) - 1, 0);
                if (tmp >= 0)
                {
                    response = (char*)realloc(response, received + tmp + 1);
                    sprintf(response + strlen(response), "%s", buffer);
                    response[received + tmp] = 0; //Byte ZERO de ket thuc xau
                    received += tmp;
                }
            } while ((tmp >= 0) && (tmp == sizeof(buffer) - 1));
            printf("%s\n", response);
            free(response);
            response = NULL;
        }
    }
}