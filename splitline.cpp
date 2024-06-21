// splitline.cpp
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include "smsh.h"
#include <algorithm>

using namespace std;

extern char *next_cmd(const char *prompt, FILE *fp);
extern void fatal(const char *s1, const char *s2, int n);
extern void *emalloc(std::size_t n);
extern void *erealloc(void *p, std::size_t n);
extern char **splitline(const std::string &line);

// 判断字符是否为分隔符
bool is_delim(char ch)
{
    return ch == ' ' || ch == '\t';
}

// 将字符串按分隔符分割成多个子字符串
char **splitline(const std::string &line)
{
    const char *delim = " \t\n";
    char **tokens = nullptr;
    char *str = strdup(line.c_str());
    char *token;

    int count = 0;
    token = strtok(str, delim);
    while (token != nullptr)
    {
        ++count;
        token = strtok(nullptr, delim);
    }

    tokens = static_cast<char **>(malloc((count + 1) * sizeof(char *)));
    if (!tokens)
    {
        free(str);
        return nullptr;
    }

    token = strtok(str, delim);
    int index = 0;
    while (token != nullptr)
    {
        tokens[index++] = strdup(token);
        token = strtok(nullptr, delim);
    }
    tokens[index] = nullptr;

    free(str);
    return tokens;
}

// 从文件流中读取下一行命令
char *next_cmd(const char *prompt, FILE *fp)
{
    stringstream buffer;
    char buf[BUFSIZ];
    int c;

    cout << prompt;
    while ((c = getc(fp)) != EOF)
    {
        if (c == '\n')
            break;
        buffer.put(c);
    }

    if (buffer.str().empty())
        return nullptr;

    char *cmdline = static_cast<char *>(emalloc(buffer.str().size() + 1));
    strcpy(cmdline, buffer.str().c_str());
    return cmdline;
}

// 释放字符串数组
void freelist(char **list)
{
    delete[] list;
}

// 创建一个新的字符串
char *newstr(const char *s, size_t l)
{
    char *rv = static_cast<char *>(emalloc(l + 1));
    strncpy(rv, s, l);
    rv[l] = '\0';
    return rv;
}

// 安全的内存分配
void *emalloc(std::size_t n)
{
    void *rv = malloc(n);
    if (!rv)
        fatal("out of memory", "", 1);
    return rv;
}

// 安全的内存重新分配
void *erealloc(void *p, std::size_t n)
{
    void *rv = realloc(p, n);
    if (!rv)
        fatal("realloc() failed", "", 1);
    return rv;
}