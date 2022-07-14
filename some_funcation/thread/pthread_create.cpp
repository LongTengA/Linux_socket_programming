#include <cstddef>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
struct arg {
    int arg1 = 1;
    int arg2 = 1000;
} fd;
void* thread_main(void* args)
{
    int i = 0;
    printf("args1 = %d\nargs2 = %d\n", ((arg*)args)->arg1, ((arg*)args)->arg2);
    sleep(2);
    return (void*)100;
}

int main()
{
    pthread_t p_pid;

    if (pthread_create(&p_pid, NULL, thread_main, (void*)&fd) != 0) {
        perror("fail :");
        return 1;
    }
    int returnval = 0;
    pthread_join(p_pid, (void**)&returnval);
    printf("returnval = %d\n", returnval);
}