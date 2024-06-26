#include <iostream>
#include <string>
#include <cctype>
#include "smsh.h"
#include "varlib.h"

using namespace std;

extern void VLlist();

// 处理赋值语句
int assign(const string &str);
// 检查变量名是否合法
bool okname(const string &str);

// 处理内置命令
int builtin_command(char **args, int *resultp)
{
    int rv = 0;

    if (strcmp(args[0], "set") == 0)
    {
        VLlist();
        *resultp = 0;
        rv = 1;
    }
    else if (strchr(args[0], '=') != NULL)
    {
        *resultp = assign(string(args[0]));
        if (*resultp != -1)
        {
            rv = 1;
        }
    }
    else if (strcmp(args[0], "export") == 0)
    {
        if (args[1] != NULL && okname(string(args[1])))
        {
            *resultp = VLexport(args[1]);
        }
        else
        {
            *resultp = 1;
        }
        rv = 1;
    }
    return rv;
}

// 处理赋值语句
int assign(const string &str)
{
    string::size_type pos = str.find('=');
    if (pos == string::npos)
        return -1;
    string name = str.substr(0, pos);
    string value = str.substr(pos + 1);

    int rv = (okname(name) ? VLstore(name.c_str(), value.c_str()) : -1);
    return rv;
}

// 检查变量名是否合法
bool okname(const string &str)
{
    for (auto ch : str)
    {
        if (isdigit(ch) && &ch == &str[0])
            return false;
        if (!isalnum(ch) && ch != '_')
            return false;
    }
    return !str.empty();
}