#include <csignal>
#include <cstdio>
#include <unistd.h>
void sigan_handler_(int)
{
    printf("\npressed Ctrl + C\n");
    _exit(1);
}

int main()
{
    struct sigaction act;
    act.sa_handler = sigan_handler_;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGINT, &act, 0);
    while (1)
        ;
}