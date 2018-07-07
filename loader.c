#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "memory.h"

#define HEAD_RECORD_NOT_FOUND 2
#define END_RECORD_NOT_FOUND  3

typedef struct record {
	int addr;
	int length;
	char bytes[0x3D];
}record;

int error_flag = 0;
int line_num = 0;
int prog_addr = 0;

int progaddr(char** args) {
	char* temp;

	if (isHex(args[1])) {
		prog_addr = strtol(args[1], &temp, 16);
		fprintf(stdout, "progaddr: program address set %06X\n", prog_addr);
		return 1;
	}
	else {
		fprintf(stderr, "progaddr: invalid address\n");
		return 0;
	}
}

void load(record record_line) {
	char* temp;
	char c[3];
	int target_addr;
	int i;

	target_addr = record_line.addr + prog_addr;

	for (i = 0; i < record_line.length; i++) {
		strncpy(c, record_line.bytes + i * 2, 2);
		memory[target_addr++] = (unsigned char)strtol(c, &temp, 16);
	}
}

void modify(int addr, int byte, int modifying_val) {
	unsigned char memory_copy[5];
	int memory_content;
	int divisor, quotient;
	int i;

	strncpy(memory_copy, memory + addr, (byte + 1) / 2);

	memory_content = (int)memory_copy[0];
	for (i = 1; i < (byte + 1) / 2; i++) {
		memory_content *= 0x100;
		memory_content += (int)memory_copy[i];
	}

	divisor = 0x1;
	for (i = 0; i < byte; i++) {
		divisor *= 0x10;
	}

	quotient = memory_content / divisor;
	memory_content = memory_content % divisor;
	memory_content += modifying_val;
	memory_content = memory_content % divisor;
	memory_content = (quotient * divisor) + memory_content;

	for (i = (byte + 1) / 2 - 1; i >= 0; i--) {
		memory_copy[i] = (unsigned char)(memory_content % 0x100);
		memory_content /= 0x100;
	}

	strncpy(memory + addr, memory_copy, (byte + 1) / 2);
}

record read_line(char* line) {
	record line_record;
	char* temp;
	char addr[7];
	char length[3];
	
	strncpy(addr, line + 1, 6);
	strncpy(length, line + 7, 2);

	line_record.addr = strtol(addr, &temp, 16);
	line_record.length = strtol(length, &temp, 16);
	strcpy(line_record.bytes, line + 9);


	return line_record;
}

int loader(char** args) {
	FILE* fp;
	record line_record;
	char line[257];

	error_flag = 0;

	if (!(fp = fopen(args[1], "r"))) {
		fprintf(stderr, "loader: failed to open file %s\n", args[1]);
		return 0;
	}

	fgets(line, sizeof(line), fp);
	if (line[0] != 'H') {
		error_flag = HEAD_RECORD_NOT_FOUND;
	}

	fgets(line, sizeof(line), fp);
	line_record = read_line(line);
	while (error_flag == 0) {
		if (line[0] == 'E') {
			break;
		}
		else if (feof(fp)) {
			error_flag = END_RECORD_NOT_FOUND;
		}
		else if (line[0] == 'T') {
			load(line_record);
		}
		else if (line[0] == 'M') {
			modify(line_record.addr + prog_addr, line_record.length, prog_addr);
		}

		fgets(line, sizeof(line), fp);
		line_record = read_line(line);
	}

	fclose(fp);

	return 1;
}
