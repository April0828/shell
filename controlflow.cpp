// controlflow.cpp
#include <iostream>
#include <cstring>
#include "smsh.h"

enum states
{
    NEUTRAL,
    WANT_THEN,
    THEN_BLOCK
};
enum results
{
    SUCCESS,
    FAIL
};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(const char *);

// 检查是否可以执行命令
int ok_to_execute()
{
    int rv = 1;

    if (if_state == WANT_THEN)
    {
        syn_err("then expected");
        rv = 0;
    }
    else if (if_state == THEN_BLOCK && if_result == SUCCESS)
    {
        rv = 1;
    }
    else if (if_state == THEN_BLOCK && if_result == FAIL)
    {
        rv = 0;
    }
    return rv;
}

// 检查是否是控制命令
int is_control_command(const char *s)
{
    return (std::strcmp(s, "if") == 0 || std::strcmp(s, "then") == 0 || std::strcmp(s, "fi") == 0);
}

// 执行控制命令
int do_control_command(char **args)
{
    const char *cmd = args[0];
    int rv = -1;

    if (std::strcmp(cmd, "if") == 0)
    {
        if (if_state != NEUTRAL)
            rv = syn_err("if unexpected");
        else
        {
            last_stat = process(args + 1);
            if_result = (last_stat == 0 ? SUCCESS : FAIL);
            if_state = WANT_THEN;
            rv = 0;
        }
    }
    else if (std::strcmp(cmd, "then") == 0)
    {
        if (if_state != WANT_THEN)
            rv = syn_err("then unexpected");
        else
        {
            if_state = THEN_BLOCK;
            rv = 0;
        }
    }
    else if (std::strcmp(cmd, "fi") == 0)
    {
        if (if_state != THEN_BLOCK)
            rv = syn_err("fi unexpected");
        else
        {
            if_state = NEUTRAL;
            rv = 0;
        }
    }
    else
        fatal("internal error processing:", cmd, 2);
    return rv;
}

// 处理语法错误
int syn_err(const char *msg)
{
    if_state = NEUTRAL;
    std::cerr << "syntax error: " << msg << std::endl;
    return -1;
}