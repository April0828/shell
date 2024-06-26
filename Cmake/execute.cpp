// execute.cpp
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <cstring>

// 执行外部命令的函数
int execute(char *argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    int child_info = -1;

    // 如果命令为空，直接返回
    if (argv[0] == NULL)
        return 0;

    // 初始化STARTUPINFO和PROCESS_INFORMATION结构
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // 构建命令行字符串
    std::string commandLine;
    for (int i = 0; argv[i] != NULL; ++i)
    {
        commandLine += argv[i];
        commandLine += " ";
    }

    // 创建子进程
    if (!CreateProcess(
            NULL,           // 不指定模块名称（使用命令行）
            &commandLine[0],// 命令行字符串
            NULL,           // 不继承进程句柄
            NULL,           // 不继承线程句柄
            FALSE,          // 不继承句柄
            0,              // 没有创建标志
            NULL,           // 使用父进程的环境变量
            NULL,           // 使用父进程的当前目录
            &si,            // STARTUPINFO结构
            &pi)            // PROCESS_INFORMATION结构
    )
    {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return -1;
    }

    // 等待子进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 获取子进程的退出码
    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode))
    {
        child_info = static_cast<int>(exitCode);
    }

    // 关闭进程和线程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return child_info;
}
