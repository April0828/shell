#ifndef SMSH_H
#define SMSH_H

#include <cstddef>
#include <cstdlib>
#include <new>
#include <vector>

constexpr int YES = 1;
constexpr int NO = 0;

char *next_cmd(const char *, FILE *);
char **splitline(const std::string &line);
void freelist(char **);
void *emalloc(std::size_t);
void *erealloc(void *, std::size_t);
int execute(char **);
void fatal(const char *, const char *, int);

int process(char **args);

#endif
