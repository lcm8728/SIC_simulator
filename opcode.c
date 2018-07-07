#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.h"

opcode_ptr opcode_table[20];
char* filename = "opcode.txt";

int makeOpTab(){
	opcode_ptr temp;
	char name[10];
	char type[4];
	int value;
	FILE* fp;
	int index;

	if(!(fp = fopen(filename, "r"))){
		fprintf(stderr, "opcode: cannot open %s\n", filename);
		return 0;
	}

	while (!feof(fp)) {
		// read value, name, type
		fscanf(fp, "%2x", &value);
		if (feof(fp)) {
			break;
		}
		fscanf(fp, "%s", name);
		fscanf(fp, "%s", type);

		if (!(temp = (opcode_ptr)malloc(sizeof(opcode_list)))) {
			// ERROR : memory allocation failure
			printf("opcode: cannot allocate memory for opcode table!\n");
			return 0;
		}

		temp->value = value;
		strcpy(temp->type, type);
		strcpy(temp->name, name);

		// hash function
		index = ((int)name[0] + (int)name[1]) % 20;

		// insert node
		if (!opcode_table[index]) {
			opcode_table[index] = temp;
			temp->next = NULL;
		}
		else {
			temp->next = opcode_table[index];
			opcode_table[index] = temp;
		}
	}
	fclose(fp);

	return 1;
}

int opcode(char** args){
	int index;
	opcode_ptr temp;

	// hash function
	index = ((int)(*args[1]) + (int)(*(args[1]+1))) % 20;
	temp = opcode_table[index];
	while (temp) {
		if (!strcmp(temp->name, args[1])) {
			printf("opcode is %02X\n", temp->value);
			return 1;
		}
		temp = temp->next;
	}
	printf("opcode \"%s\" not found\n", args[1]);

	return 1;
}

int opcodelist(char** args){
	int i;
	opcode_ptr temp;

	for (i = 0; i < 20; i++) {
		printf("%2d : ", i);
		temp = opcode_table[i];
		if(!temp){
			printf("empty");
		}
		while (temp) {
			printf("[%s, %X]", temp->name, temp->value);
			temp = temp->next;
			if (temp) {
				printf(" -> ");
			}
		}
		printf("\n");
	}

	return 1;
}

int inOptab(char* opcode) {
	opcode_ptr temp;
	int index;
	int extended;
	int found = 0;

	if (opcode[0] == '+') {
		extended = 1;
		opcode = opcode + 1;
	}

	index = ((int)opcode[0] + (int)opcode[1]) % 20;
	temp = opcode_table[index];
	
	while (temp) {
		if (strcmp(opcode, temp->name) == 0) {
			found = 1;
			break;
		}
		temp = temp->next;
	}

	if (found) return 1;
	else return 0;
}

int opcodeType(char* opcode) {
	opcode_ptr temp;
	int idx;
	int extended = 0;

	if (opcode[0] == '+') {
		extended = 1;
		opcode = opcode + 1;
	}

	idx = ((int)opcode[0] + (int)opcode[1]) % 20;
	temp = opcode_table[idx];
	while (temp) {
		if (strcmp(temp->name, opcode) == 0) {
			if (strcmp(temp->type, "1") == 0) return 1;
			else if (strcmp(temp->type, "2") == 0) return 2;
			else if (extended == 0) return 3;
			else return 4;
		}
		temp = temp->next;
	}

	return -1;
}

int opcodeValue(char* opcode) {
	opcode_ptr temp;
	int idx;

	if (opcode[0] == '+') {
		opcode = opcode + 1;
	}

	idx = ((int)opcode[0] + (int)opcode[1]) % 20;
	temp = opcode_table[idx];
	while (temp) {
		if (strcmp(temp->name, opcode) == 0) {
			return temp->value;
		}
		temp = temp->next;
	}

	return -1;
}
