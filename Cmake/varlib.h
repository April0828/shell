#ifndef VARLIB_H
#define VARLIB_H

#include <cstring>
#include <cstdlib>

#ifndef MAXVARS
#define MAXVARS 200
#endif

using std::size_t;

struct Var;

int VLenviron2table(char **);
int VLexport(const char *);
char *VLlookup(const char *);
void VLlist();
int VLstore(const char *, const char *);
char **VLtable2environ();

#endif
