#include <cstddef>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
//互斥锁 mutex

int nums = 0;
pthread_mutex_t mutex;
void* add(void*);
void* sub(void*);
void* info(void*);
int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t p[10];
    int i = 0;
    for (; i < 10; i++) {
        if (i % 2)
            pthread_create(&p[i], NULL, add, NULL);
        else
            pthread_create(&p[i], NULL, sub, NULL);
    }
    for (i = 0; i < 10; i++)
        pthread_join(p[i], NULL);

    printf("\n%d\n", nums);
}

void* info(void*)
{
    int i = 0;
    while (1) {
        printf("\r%d", i++);
    }
}
void* add(void*)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 100; i++)
        ++nums;
    pthread_mutex_unlock(&mutex);
}
void* sub(void*)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 100; i++)
        --nums;
    pthread_mutex_unlock(&mutex);
}
