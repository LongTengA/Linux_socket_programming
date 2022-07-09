//wait 函数
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        return 89;
    } else {
        pid = fork();
        if (pid == 0) {
            sleep(15);
            return 7;
        } else {
            sleep(20);
            int id = waitpid(-1, &status, WNOHANG);
            // wait(&status);
            // if (WIFEXITED(status)) {
            printf("child sent %d\n", WEXITSTATUS(status));
            id = waitpid(-1, &status, WNOHANG);
            printf("child sent %d\n", WEXITSTATUS(status));
            //     fflush(stdout);
            // }
            // wait(&status);
            // if (WIFEXITED(status)) {
            //     printf("child sent %d\n", WEXITSTATUS(status));
            //     fflush(stdout);
            // }
        }
    }
    return 0;
}

int show_info()
{
    //fork()产生一个新的进程 复制父进程的所有资源 子进程pid = 0 使用条件分支来编写子进程功能
    int pid = fork();
    //wait()等待子进程结束 会阻塞 将子进程结束的信息存入status变量中
    int status;
    wait(&status);
    //这个宏用于检查是否有返回状态
    WIFEXITED(status);
    //这个宏用于 返回 子进程退出的返回值
    WEXITSTATUS(status);
    //这个当子进程正常结束时返回真
    WEXITED;
    //参数填-1 则wait所有的子进程结束，否则是指定的pid 第二个status
    //waitpid(__pid_t pid, int *stat_loc, int options)
    //第三个参数
    //#define WNOHANG 1 /* Don't block waiting.  */设置waitpid不阻塞
    //#define WUNTRACED 2 /* Report status of stopped children.  */
}