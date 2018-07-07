#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "util.h"

char memory[MEMORY_SIZE];
int dump_address = 0;

int dump(char** args) {
	int i, index;
	int start_address, end_address;
	int start_row, end_row;
	char* temp;

	start_address = dump_address;
	start_row = (start_address / 0x10) * 0x10;
	
	// start address given
	if (args[1]) {
		// str -> int
		start_address = strtol(args[1], &temp, 16);
		// ERROR : Values not hexadecimal
		if (!isHex(args[1]) || start_address >= MEMORY_SIZE || start_address < 0) {
			fprintf(stderr, "dump: invalid address \"%s\"\n", args[1]);
			return 0;
		}
		start_row = (start_address / 0x10) * 0x10;
	}

	end_address = start_address + 0x9F;
	end_row = (end_address / 0x10) * 0x10;

	// end address given
	if (args[2]) {
		// str -> int
		end_address = strtol(args[2], &temp, 16);
		// ERROR : Address value over memory size
		//         Or start address larger than end address
		if (!isHex(args[2]) || end_address < 0 || end_address >= MEMORY_SIZE) {
			fprintf(stderr, "dump: invalid address \"%s\"\n", args[2]);
			return 0;
		}
		if (end_address < start_address){
			fprintf(stderr, "dump: end address must be bigger than start address\n");
			return 0;
		}

		end_row = (end_address / 0x10) * 0x10;
	}

	// print memory
	for (; start_row <= end_row; start_row += 0x10) {
		if (start_row < MEMORY_SIZE) {
			printf("%05X", start_row);
			// print hexadecimal values
			for (i = 0; i < 0x10; i++) {
				index = start_row + i;
				if (index >= start_address && index <= end_address) {
					printf(" %02X", memory[index] & 0xFF);
				}
				else {
					printf("   ");
				}
			}
			printf(" ; ");
			// print ASCII
			for (i = 0; i < 0x10; i++) {
				index = start_row + i;
				if (memory[index] >= 0x20 && memory[index] <= 0x7E 
						&& index >= start_address && index <= end_address) {
					printf("%c", memory[index]);
				}
				else {
					printf(".");
				}
			}
			printf("\n");
		}
		// Out of memory size
		else {
			break;
		}
	}
	
	// set next dump address
	dump_address = end_address + 1;
	if (dump_address >= MEMORY_SIZE) dump_address = 0;

	return 1;
}

int edit(char** args) {
	int addr, value;
	char* temp;

	// ERROR : arguments not hex
	if (!isHex(args[1])) {
		printf("edit: invalid address \"%s\"\n", args[1]);
		return 0;
	}
	if (!isHex(args[2])) {
		printf("edit: invalid value \"%s\"\n", args[2]);
		return 0;
	}

	// str -> int
	addr = strtol(args[1], &temp, 16);
	value = strtol(args[2], &temp, 16);

	// ERROR : arguments out of range
	if (addr >= MEMORY_SIZE || addr < 0) {
		printf("edit: invalid address \"%s\"\n", args[1]);
		return 0;
	}
	if (value > 0xFF || value < 0) {
		printf("edit: invalid value \"%s\"\n", args[2]);
		return 0;
	}

	memory[addr] = value;

	return 1;
}

int fill(char** args) {
	int start, end, value;
	char* temp;
	
	// ERROR : arguments not hexadecimal
	if (!isHex(args[1])) {
		printf("fill: invalid address \"%s\"\n", args[1]);
		return 0;
	}
	if (!isHex(args[2])) {
		printf("fill: invalid address \"%s\"\n", args[2]);
		return 0;
	}
	if (!isHex(args[3])) {
		printf("fill: invalid value \"%s\"\n", args[3]);
		return 0;
	}

	// str -> int
	start = strtol(args[1], &temp, 16);
	end = strtol(args[2], &temp, 16);
	value = strtol(args[3], &temp, 16);

	// ERROR : arguments out of range
	if (start >= MEMORY_SIZE || start < 0 || end >= MEMORY_SIZE || end < 0) {
		printf("fail: invalid address start: \"%s\", end: \"%s\"\n", args[1], args[2]);
		return 0;
	}
	if (end < start){
		printf("fail: end address must be bigger than start address\n");
		return 0;
	}
	if (value > 0xFF || value < 0) {
		printf("fail: invalid value \"%s\"\n", args[3]);
		return 0;
	}

	for (; start <= end; start++) {
		memory[start] = value;
	}

	return 1;
}	

int reset(char**args) {
	int i;

	for(i = 0; i < MEMORY_SIZE; i++){
		memory[i] = 0;
	}

	return 1;
}

