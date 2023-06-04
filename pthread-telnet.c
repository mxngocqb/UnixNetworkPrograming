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
pthread_mutex_t *mutex = NULL;

void *telnet(void *arg)
{
    int c = *(int *)arg;
    free(arg);
    arg = NULL;
    char buffer[1024];
    while (0 == 0)
    {
        memset(buffer, 0, sizeof(buffer));
        int r = recv(c, buffer, sizeof(buffer) - 1, 0);
        if (r > 0)
        {
            if (buffer[strlen(buffer) - 1] == '\n')
            {
                buffer[strlen(buffer) - 1] = 0;
            }
            pthread_mutex_lock(mutex);
            sprintf(buffer + strlen(buffer), " > command.txt");
            printf("%s", buffer);
            system(buffer);
            FILE *f = fopen("command.txt", "rb");
            fseek(f, 0, SEEK_END);
            int size = ftell(f);
            char *data = (char *)calloc(size, 1);
            fseek(f, 0, SEEK_SET);
            fread(data, 1, size, f);
            pthread_mutex_unlock(mutex);
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
        }
        else
            break;
    }
    printf("%d disconnected\n", c);
    return NULL;
}
int main()
{
    mutex = (pthread_mutex_t *)calloc(1, sizeof(pthread_mutex_t));
    // Khởi tạo 1 mutex
    pthread_mutex_init(mutex, NULL);
    SOCKADDR_IN myaddr, caddr;
    int clen = sizeof(caddr);
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    myaddr.sin_family = AF_INET;                  // IPv4
    myaddr.sin_port = htons(8888);                // Random port 50000
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
        pthread_create(&tid, NULL, telnet, (void *)arg); // create a new thread
    }
    pthread_mutex_destroy(mutex);
    free(mutex);
}