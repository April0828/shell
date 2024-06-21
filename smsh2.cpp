// smsh2.cpp
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "smsh.h"
#include <vector>

using namespace std;

// 声明外部函数，用于获取下一条命令和分割命令行
extern char *next_cmd(const char *prompt, FILE *fp);
extern char **splitline(const std::string &line);

// 默认提示符
string DFL_PROMPT = "> ";

// 主函数，程序入口
int main()
{
    char *cmdline, *prompt;
    char **arglist;
    int result;
    void setup();

    const char *constc = nullptr;
    constc = DFL_PROMPT.c_str();
    prompt = const_cast<char *>(constc);

    setup();

    while (true)
    {
        cmdline = next_cmd(prompt, stdin);
        if (cmdline == NULL)
            break;

        arglist = splitline(cmdline);
        if (arglist != NULL)
        {
            result = process(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }

    return 0;
}

// 设置信号处理函数
void setup()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

// 错误处理函数，输出错误信息并退出程序
void fatal(const char *s1, const char *s2, int n)
{
    cerr << "Error: " << s1 << "," << s2 << endl;
    exit(n);
}