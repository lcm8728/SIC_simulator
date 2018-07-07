
#ifndef _RUN_CMD_H_
#define _RUN_CMD_H_

struct cmd_struct{
	char *name;
	int(*func)(char**);
	int min_arg;
	int max_arg;
};

void runCmd(char**);

#endif 

