#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int S1 = 0, S2 = 0;
int N = 20000;

pthread_mutex_t *mutex = NULL;

void *AddOne(void *arg)
{
    int i = *((int *)arg);
    pthread_mutex_lock(mutex);
    S2 += i;
    pthread_mutex_unlock(mutex);
    arg = NULL;
    free(arg);
    usleep(10000);
    return NULL;
}

int main()
{

    mutex = (pthread_mutex_t *)calloc(1, sizeof(pthread_mutex_t));
    // Khởi tạo 1 mutex
    pthread_mutex_init(mutex, NULL);

    pthread_t *tid = (pthread_t *)calloc(N, sizeof(pthread_t));

    // Hàm cộng bình thường
    for (int i = 0; i < N; i++)
    {
        S1 += i;
    }
    // Hàm công vip pro
    for (int i = 0; i < N; i++)
    {
        int *arg = (int *)calloc(1, sizeof(int));
        *arg = i;
        pthread_create(&tid[i], NULL, AddOne, ((void *)arg));
    }

    int *status = NULL;
    // Đồng bộ tiến trình
    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], (void **)&status);
    }

    pthread_mutex_destroy(mutex);
    free(tid);
    free(mutex);
    tid = NULL;
    printf("%d %d\n", S1, S2);
}