#include <stdio.h>
#include <string.h>
#include "run_cmd.h"
#include "shell_cmd.h"
#include "history.h"
#include "memory.h"
#include "opcode.h"
#include "assemble.h"
#include "loader.h"

#define TOO_MANY_ARGS 1
#define TOO_FEW_ARGS 2

struct cmd_struct cmd_list[] = {
	{"help", help, 1, 1},
	{"h", help, 1, 1},
	{"dir", dir, 1, 1},
	{"d", dir, 1, 1},
	{"quit", quit, 1, 1},
	{"q", quit, 1, 1},
	{"history", history, 1, 1},
	{"hi", history, 1, 1},
	{"dump", dump, 1, 3},
	{"du", dump, 1, 3},
	{"edit", edit, 1, 3},
	{"e", edit, 1, 3},
	{"fill", fill, 1, 4},
	{"f", fill, 1, 4},
	{"reset", reset, 1, 1},
	{"opcode", opcode, 2, 2},
	{"opcodelist", opcodelist, 1, 1},
	{"assemble", assemble, 2, 2},
	{"type", type, 2, 2},
	{"loader", loader, 2, 2 },
	{"progaddr", progaddr, 2, 2}
};

int countArg(char** args){
	int i, count;
	for(i = 0, count = 0; i < 5; i++){
		if(args[i] != NULL){
			count++;
		}
	}

	return count;
}

void runCmd(char**args){
	int i, num_arg;

	num_arg = countArg(args);

	for(i = 0; i < sizeof(cmd_list)/sizeof(struct cmd_struct); i++){
		if(strcmp(args[0], cmd_list[i].name) == 0){
			if(num_arg < cmd_list[i].min_arg){
				fprintf(stderr, "%s: too few arguments\n", cmd_list[i].name);
			}
			else if(num_arg > cmd_list[i].max_arg){
				fprintf(stderr, "%s: too many arguments\n", cmd_list[i].name);
			}
			else{
				addHistory();
				cmd_list[i].func(args);
			}
			return;
		}
	}

	fprintf(stderr, "No command \"%s\" found\n", args[0]);
}

