//进程间通信
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int pipefd[2] { 0 };
    char buffer[512];
    //创建好了管道,分别为入和出，这个管道存在与操作系统内核
    pipe(pipefd);
    pid_t pid = fork();
    if (pid == 0) {
        while (1 && *buffer != 'Q') {
            int size = read(pipefd[0], buffer, 512);
            if (size > 0) {
                printf("recv from father:\n%s", buffer);
                fflush(stdin);
            }
        }
        _exit(1);
    } else {
        while (1 && *buffer != 'Q') {
            int size = read(1, buffer, 512);
            write(pipefd[1], buffer, size);
        }
    }
}
