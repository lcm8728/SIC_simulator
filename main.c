#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run_cmd.h"
#include "history.h"
#include "opcode.h"

#define MAXLEN 258

char** parseString(char* line){
	char** args;
	char* token;
	int index;

	index = 0;
	args = (char**)malloc(sizeof(char*) * 5);
	// set args null for counting number of arguments
	for(index = 0; index < 5; index++){
		args[index] = NULL;
	}

	// arguments are separated by space(' ') or tab('\t')
	index = 0;
	token = strtok(line, " \t\n");
	while(token){
		if(index >= 5) break;
		args[index++] = token;
		token = strtok(NULL, " \t\n");
	}

	return args;
}

void getLine(char* line) {
	fgets(line, MAXLEN, stdin);
	// get new line if input line is too long or empty
	while(strlen(line) > 256 || strlen(line) == 1) {
		printf("sicsim>");
		fgets(line, MAXLEN, stdin);
	}
}

int main(){
	char line[MAXLEN];
	char** args;
	
	makeOpTab();
	while(1){
		printf("sicsim>");
		getLine(line);
		tempHistory(line);
		args = parseString(line);
		runCmd(args);

		free(args);
	}

	return 0;
}
