
#ifndef _MEMORY_H_
#define _MEMORY_H_

#define MEMORY_SIZE 1048576

int dump(char**);
int edit(char**);
int fill(char**);
int reset(char**);

extern char memory[MEMORY_SIZE];

#endif
