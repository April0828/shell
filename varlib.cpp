// varlib.cpp
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <new>
#define MAXVARS 200

struct Var
{
    char *str;
    bool global;
};

static Var tab[MAXVARS];

static char *new_string(const char *name, const char *val);
static Var *find_item(const char *name, int first_blank);

// 存储变量名和值到变量表中
int VLstore(const char *name, const char *val)
{
    Var *itemp;
    char *s;
    int rv = 1;

    if ((itemp = find_item(name, 1)) != nullptr && (s = new_string(name, val)) != nullptr)
    {
        if (itemp->str)
        {
            delete[] itemp->str;
        }
        itemp->str = s;
        itemp->global = false;
        rv = 0;
    }
    return rv;
}

// 创建一个新的字符串，格式为 "name=value"
char *new_string(const char *name, const char *val)
{
    size_t len = std::strlen(name) + std::strlen(val) + 2;
    char *retval = new char[len];
    if (retval != nullptr)
    {
        std::sprintf(retval, "%s=%s", name, val);
    }
    return retval;
}

// 查找并返回变量的值
char *VLlookup(const char *name)
{
    Var *itemp;

    if ((itemp = find_item(name, 0)) != nullptr)
    {
        return itemp->str + std::strlen(name) + 1;
    }
    return const_cast<char *>("");
}

// 将变量标记为全局变量
int VLexport(const char *name)
{
    Var *itemp;
    int rv = 1;

    if ((itemp = find_item(name, 0)) != nullptr)
    {
        itemp->global = true;
        rv = 0;
    }
    else if (VLstore(name, "") == 0)
    {
        rv = VLexport(name);
    }
    return rv;
}

// 查找变量项，如果 first_blank 为真，则返回第一个空项
static Var *find_item(const char *name, int first_blank)
{
    int i;
    int len = strlen(name);

    for (i = 0; i < MAXVARS && tab[i].str != nullptr; i++)
    {
        if (std::strncmp(tab[i].str, name, std::strlen(name)) == 0 && tab[i].str[std::strlen(name)] == '=')
        {
            return &tab[i];
        }
    }
    if (first_blank && i < MAXVARS)
    {
        return &tab[i];
    }
    return nullptr;
}

// 列出所有变量，全局变量前加星号
void VLlist()
{
    for (int i = 0; i < MAXVARS && tab[i].str != nullptr; i++)
    {
        if (tab[i].global)
        {
            std::cout << "  * " << tab[i].str << std::endl;
        }
        else
        {
            std::cout << "    " << tab[i].str << std::endl;
        }
    }
}

// 将环境变量导入到变量表中
int VLenviron2table(const char **env)
{
    int i;
    char *newstring;

    for (i = 0; env[i] != nullptr; ++i)
    {
        if (i == MAXVARS)
            return 0;
        newstring = new char[strlen(env[i]) + 1];
        if (newstring == nullptr)
            return 0;
        strcpy(newstring, env[i]);
        tab[i].str = newstring;
        tab[i].global = true;
    }
    for (; i < MAXVARS; ++i)
    {
        tab[i].str = nullptr;
        tab[i].global = false;
    }
    return 1;
}

// 将变量表转换为环境变量数组
char **VLtable2environ()
{
    int i, j, n = 0;

    for (i = 0; i < MAXVARS && tab[i].str != nullptr; ++i)
        if (tab[i].global)
            n++;

    char **envtab = new char *[n + 1];
    if (envtab == nullptr)
        return nullptr;

    for (i = 0, j = 0; i < MAXVARS && tab[i].str != nullptr; ++i)
    {
        if (tab[i].global)
            envtab[j++] = tab[i].str;
    }
    envtab[j] = nullptr;
    return envtab;
}