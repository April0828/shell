// execute.cpp
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring>

// 执行外部命令的函数
int execute(char *argv[])
{
    pid_t pid;
    int child_info = -1;

    // 如果命令为空，直接返回
    if (argv[0] == NULL)
        return 0;

    // 创建子进程
    if ((pid = fork()) == -1)
    {
        std::perror("fork");
    }
    else if (pid == 0)
    {
        // 子进程中重置信号处理
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        // 执行命令
        if (execvp(argv[0], argv) == -1)
        {
            std::perror("cannot execute command");
            std::exit(1);
        }
    }
    else
    {
        // 父进程等待子进程结束
        if (wait(&child_info) == -1)
        {
            std::perror("wait");
        }
    }
    return child_info;
}
