
#ifndef _OPCODE_H_
#define _OPCODE_H_

typedef struct opcode_list* opcode_ptr;
typedef struct opcode_list{
	char name[10];
	char type[4];
	int value;
	opcode_ptr next;
}opcode_list;

int makeOpTab();
int opcode(char**);
int opcodelist(char**);
int inOptab(char*);
int opcodeType(char*);
int opcodeValue(char*);

#endif
