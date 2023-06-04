#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int exitflag;

void *mythread(void *arg)
{
    int i = *((int *)arg); // Lấy dữ liệu vung nhớ arg
    printf("Hello from my: (%d) %d\n", (int)pthread_self(), i);
    // printf("Hello from my: %d\n",  i);
    free(arg);
    // exitflag = 1;
    return 0;
}

int main()
{
    // exitflag = 0;
    pthread_t tid;
    for (int i = 0; i < 30; i++)
    {
        int *arg = (int *)calloc(1, sizeof(int));
        *arg = i;
        pthread_create(&tid, NULL, mythread, arg);
    }
    printf("Hello from mainthread\n");
    getchar();

    // while (exitflag == 0)
    // {
    //     usleep(10000);
    // }
}