#include <csignal>
#include <cstdio>
#include <unistd.h>

void signal_handler01(int)
{
    printf("\npressed Ctrl+C\n");
    _exit(1);
}

int main()
{
    signal(SIGINT, signal_handler01);
    while (1) {
    }
}