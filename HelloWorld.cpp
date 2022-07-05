#include <string.h>
#include <unistd.h>
int main()
{
    const char* buffer = "Hello world!";
    write(1, buffer, strlen(buffer));
    return -1;
}