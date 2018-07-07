#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "opcode.h"

#define NOT_ASM_FILE		    1
#define INVALID_OPERAND			2
#define END_NOT_FOUND			3
#define SAME_SYMBOL_USED_TWICE	4
#define INVALID_OPERATION_CODE	5
#define UNDEFINED_SYMBOL		6
#define INVALID_MEMORY_ACCESS   7
#define FILE_OPEN_FAILURE       8

typedef struct symbol_node* symbol_ptr;
typedef struct symbol_node {
	char name[7];
	int address;
	symbol_ptr next;
}symbol_node;

typedef struct modification_record_node* modification_record_ptr;
typedef struct modification_record_node {
	int byte;
	int modi_type;
	modification_record_ptr next;
}modification_record_node;

typedef struct error_flag {
	int error_no;
	int line_no;
}error_flag;

typedef struct words {
	char original_line[257];
	char symbol_node[8];
	char opcode[8];
	char operand1[10];
	char operand2[10];
}words;

error_flag flag;
symbol_ptr symbol_tab = NULL;
modification_record_ptr modification_record;
int line_num;

void reset_error_flag() {
	flag.error_no = 0;
}

void set_error_flag(int _error_no, int _line_no) {
	flag.error_no = _error_no;
	flag.line_no = _line_no;
}

int check_error(){
	if (flag.error_no != 0) return 1;
	else return 0;
}

void print_error() {
	switch (flag.error_no) {
	case(NOT_ASM_FILE):	           fprintf(stderr, "assemble: input file not .asm file\n"); break;
	case(INVALID_OPERAND):         fprintf(stderr, "assemble: invalid operand; line %d\n", flag.line_no); break;
	case(END_NOT_FOUND) :          fprintf(stderr, "assemble: END not found; line %d\n", flag.line_no); break;
	case(SAME_SYMBOL_USED_TWICE) : fprintf(stderr, "assemble: same symbol used twice; line %d\n", flag.line_no); break;
	case(INVALID_OPERATION_CODE) : fprintf(stderr, "assemble: invalid operation code; line %d\n", flag.line_no); break;
	case(UNDEFINED_SYMBOL) :       fprintf(stderr, "assemble: undefined symbol; line %d\n", flag.line_no); break;
	case(INVALID_MEMORY_ACCESS) :  fprintf(stderr, "assemble: invalid memory access; line %d\n", flag.line_no); break;
	case(FILE_OPEN_FAILURE):       fprintf(stderr, "assemble: failed to open file"); break;
	}
}

void clear_modification_record_table() {
	modification_record_ptr prev, next;

	prev = modification_record;
	while (prev) {
		next = prev->next;
		free(prev);
		prev = next;
	}
	modification_record = NULL;
}

void clear_symbol_table() {
	symbol_ptr prev, next;

	prev = symbol_tab;
	while (prev) {
		next = prev->next;
		free(prev);
		prev = next;
	}
	symbol_tab = NULL;
}

void insert_modi_rec(int byte, int type) {
	modification_record_ptr temp, next;

	next = modification_record;

	temp = (modification_record_ptr)malloc(sizeof(modification_record_node));
	temp->byte = byte;
	temp->modi_type = type;
	temp->next = NULL;

	if (next == NULL) modification_record = temp;
	else {
		while (next->next) {
			next = next->next;
		}
		next->next = temp;
	}
}

int inSymbolTable(char* symbol_name){
	symbol_ptr temp;

	if (symbol_name[0] == '#' || symbol_name[0] == '@') {
		symbol_name += 1;
	}

	temp = symbol_tab;
	while(temp){
		if(strcmp(symbol_name, temp->name) == 0){
			return 1;
		}
		temp = temp->next;
	}

	return 0;
}

int symbolAddress(char* symbol_name) {
	symbol_ptr temp;

	if (symbol_name[0] == '#' || symbol_name[0] == '@') {
		symbol_name += 1;
	}

	temp = symbol_tab;
	while (temp) {
		if (strcmp(symbol_name, temp->name) == 0) {
			return temp->address;
		}
		temp = temp->next;
	}

	return -1;
}

void insertSymtab(char* symbol_name, int location){
	symbol_ptr temp;
	symbol_ptr next;

	temp = (symbol_ptr)malloc(sizeof(symbol_node));
	strcpy(temp->name, symbol_name);
	temp->address = location;
	temp->next = NULL;

	if(symbol_tab == NULL){
		symbol_tab = temp;
	}
	else{
		next = symbol_tab;
		while(next->next){
			next = next->next;
		}
		next->next = temp;
	}
}

int isSymbol(char* line){
	if(line[0] == ' ' || line[0] == '\t'){
		return 0;
	}
	else{
		return 1;
	}
}

void printSymtab() {
	symbol_ptr temp;
	temp = symbol_tab;
	while (temp) {
		printf("%6s\t%06X\n", temp->name, temp->address);
		temp = temp->next;
	}
}

int isRegister(char* reg) {
	if (strspn(reg, "AXLBSTF") == strlen(reg) && strlen(reg) == 1) return 1;
	if (!strcmp(reg, "PC")) return 1;
	if (!strcmp(reg, "SW")) return 1;
	return 0;
}

int registerVal(char* reg) {
	if (!strcmp(reg, "A")) return 0;
	else if (!strcmp(reg, "X")) return 1;
	else if (!strcmp(reg, "L")) return 2;
	else if (!strcmp(reg, "B")) return 3;
	else if (!strcmp(reg, "S")) return 4;
	else if (!strcmp(reg, "T")) return 5;
	else if (!strcmp(reg, "F")) return 6;
	else if (!strcmp(reg, "PC")) return 8;
	else if (!strcmp(reg, "SW")) return 9;
	else return -1;
}

words parseLine(char line[]){
	words word;
	char* temp;

	word.original_line[0] = '\0';
	word.symbol_node[0] = '\0';
	word.opcode[0] = '\0';
	word.operand1[0] = '\0';
	word.operand2[0] = '\0';

	if (line != NULL) {
		strncat(word.original_line, line, strlen(line) - 1);
	}

	temp = strtok(line, " \t\n\r");
	if (isSymbol(line)) {
		if (temp)
			strncat(word.symbol_node, temp, sizeof(word.symbol_node) - 1);
		else
			word.symbol_node[0] = '\0';
		temp = strtok(NULL, " \t\n\r");
	}
	if (temp)
		strncat(word.opcode, temp, sizeof(word.opcode) - 1);
	else
		word.opcode[0] = '\0';

	temp = strtok(NULL, " \t\n\r,");
	if (temp)
		strncat(word.operand1, temp, sizeof(word.operand1) - 1);
	else
		word.operand1[0] = '\0';

	temp = strtok(NULL, " \t\n\r,");
	if (temp)
		strncat(word.operand2, temp, sizeof(word.operand2) - 1);
	else
		word.operand2[0] = '\0';

	return word;
}

int ignoredLine(char line[]){
	if(isEmptyLine(line)){
		return 1;
	}
	else if(line[0] == '.'){
		return 1;
	}

	return 0;
}

int location_counter(words word) {
	char* temp;
	int locctr = 0;

	if (inOptab(word.opcode)) {
		locctr += opcodeType(word.opcode);
	}
	else if (strcmp("WORD", word.opcode) == 0) {
		locctr += 3;
	}
	else if (strcmp("RESW", word.opcode) == 0) {
		if (isDec(word.operand1)) {
			locctr += strtol(word.operand1, &temp, 10) * 3;
		}
		else {
			set_error_flag(INVALID_OPERAND, line_num);
		}
	}
	else if (strcmp("BYTE", word.opcode) == 0) {
		if (word.operand1[0] == 'C') {
			temp = strtok(word.operand1 + 1, "'");
			locctr += strlen(temp);
		}
		else if (word.operand1[0] == 'X') {
			locctr += 1;
		}
		else {
			set_error_flag(INVALID_OPERAND, line_num);
		}
	}
	else if (strcmp("RESB", word.opcode) == 0) {
		if (isDec(word.operand1)) {
			locctr += strtol(word.operand1, &temp, 10);
		}
		else {
			set_error_flag(INVALID_OPERAND, line_num);
		}
	}
	else if (strcmp("BASE", word.opcode) == 0) {}
	else {
		set_error_flag(INVALID_OPERATION_CODE, line_num);
	}

	return locctr;
}

int assemble_pass1(FILE* asm_fp){
	words word;
	char* temp;
	char line[257];
	int locctr;
	int start_addr;

	line_num = 0;

	while (!check_error()) {
		fgets(line, 257, asm_fp);
		line_num++;
		if (feof(asm_fp)) {
			set_error_flag(END_NOT_FOUND, line_num);
		}
		else if (!ignoredLine(line)) {
			word = parseLine(line);

			if (strcmp(word.opcode, "START") == 0) {
				if (isHex(word.operand1)) {
					locctr = strtol(word.operand1, &temp, 16);
					start_addr = locctr;
					fgets(line, 257, asm_fp);
					line_num++;
					word = parseLine(line);
				}
				else {
					set_error_flag(INVALID_OPERAND, line_num);
				}
			}
			else {
				locctr = 0;
			}

			break;
		}
	}


 	while (!check_error()) {
		if (strcmp(word.opcode, "END") == 0) {
			break;
		}
		else if(feof(asm_fp)){
			set_error_flag(END_NOT_FOUND, line_num);
			break;
		}
		else if (!ignoredLine(line)) {
			if (isSymbol(line)) {
				if (inSymbolTable(word.symbol_node)) {
					set_error_flag(SAME_SYMBOL_USED_TWICE, line_num);
				}
				else {
					insertSymtab(word.symbol_node, locctr);
				}
			}
			locctr += location_counter(word);
		}

		fgets(line, 257, asm_fp);
		line_num++;
		if (!ignoredLine(line)) {
			word = parseLine(line);
		}
	}

	return locctr - start_addr;
}

void assemble_pass2(FILE* asm_fp, FILE* obj_fp, FILE* lst_fp, int prog_length){
	modification_record_ptr modi;
	words word;
	char* temp;
	char line[257];
	char buffer[100];
	char record_line[100];
	int obj_code;
	int current_addr;
	int pc;
	int base;
	int i;
	int first_executable_addr;
	int first_executable_inst;

	// initialize variables
	record_line[0] = '\0';
	line_num = 0;
	obj_code = 0;
	first_executable_addr = 0;
	first_executable_inst = 0;

	while (!check_error()) {
		fgets(line, 257, asm_fp);
		line_num++;
		if (!ignoredLine(line)) {
			word = parseLine(line);

			fprintf(lst_fp, "%04d\t", line_num * 5);
			fprintf(lst_fp, "%-50s\n", word.original_line);

			if (strcmp(word.opcode, "START") == 0) {
				fprintf(obj_fp, "H");
				fprintf(obj_fp, "%6s", word.symbol_node);
				fprintf(obj_fp, "%06X", strtol(word.operand1, &temp, 16));
				fprintf(obj_fp, "%06X\n", prog_length);
				pc = strtol(word.operand1, &temp, 16);
				fgets(line, 257, asm_fp);
				line_num++;
				word = parseLine(line);
			}
			else {
				fprintf(obj_fp, "H");
				fprintf(obj_fp, "NONAME");
				fprintf(obj_fp, "000000");
				fprintf(obj_fp, "%06X\n", prog_length);
				pc = 0;
			}

			break;
		}
	}


	while (!check_error()) {
		// END
		if (strcmp(word.opcode, "END") == 0) {

			fprintf(lst_fp, "%04d\t", line_num * 5);
			fprintf(lst_fp, "%-50s\n", word.original_line);

			if (record_line[0]) {
				fprintf(obj_fp, "%02X", strlen(record_line) / 2);
				fprintf(obj_fp, "%s\n", record_line);
				record_line[0] = '\0';
			}

			// modification record
			modi = modification_record;
			while (modi) {
				fprintf(obj_fp, "M%06X%02d\n", modi->byte, modi->modi_type);
				modi = modi->next;
			}

			// E record
			fprintf(obj_fp, "E%06X", first_executable_addr);

			break;
		}

		// Not comment or empty line
		else if (!ignoredLine(line)) {
			current_addr = pc;
			pc += location_counter(word);

			fprintf(lst_fp, "%04d\t", line_num * 5);
			fprintf(lst_fp, "%-50s\t", word.original_line);

			// Instruction
			if (inOptab(word.opcode)) {
				obj_code = opcodeValue(word.opcode);
				if (first_executable_inst == 0) {
					first_executable_addr = current_addr;
					first_executable_inst = 1;
				}
				// FORMAT 1
				if (opcodeType(word.opcode) == 1) {
					if (word.opcode[0] != '\0') {
						set_error_flag(INVALID_OPERAND, line_num);
					}
					sprintf(buffer, "%02X", obj_code);
				}

				// FORMAT 2
				else if (opcodeType(word.opcode) == 2) {
					obj_code *= 0x100;
					if (isRegister(word.operand1)) {
						obj_code += registerVal(word.operand1) * 0x10;
						if (word.operand2[0] && isRegister(word.operand2)) {
							obj_code += registerVal(word.operand2);
						}
					}
					else {
						set_error_flag(INVALID_OPERAND, line_num);
					}
					sprintf(buffer, "%04X", obj_code);
				}

				// FORMAT 3
				if (opcodeType(word.opcode) == 3) {
					obj_code *= 0x10000;

					// indexed mode
					if (strcmp(word.operand2, "X") == 0) {
						obj_code += 0x8000;
					}

					// n, i
					if (word.operand1[0] == '@') {
						if (inSymbolTable(word.operand1)) {
							// indirect addressing
							obj_code += 0x20000;
						}
						else {
							set_error_flag(INVALID_OPERAND, line_num);
						}
					}
					else if (word.operand1[0] == '#') {
						// immediate addressing
						obj_code += 0x10000;
						if (isDec(word.operand1 + 1)) {
							obj_code += strtol(word.operand1 + 1, &temp, 10);
						}
						else if (inSymbolTable(word.operand1)) {}
						else {
							set_error_flag(INVALID_OPERAND, line_num);
						}
					}
					else {
							obj_code += 0x30000;
					}

					// pc or base relative
					if (inSymbolTable(word.operand1)) {
						if (symbolAddress(word.operand1) - pc <= 2047 && symbolAddress(word.operand1) - pc >= -2048) {
							obj_code += 0x2000;
							obj_code += 0x00000FFF & (symbolAddress(word.operand1) - pc);
						}
						else if (symbolAddress(word.operand1) - base <= 4095 && symbolAddress(word.operand1) - base >= 0) {
							obj_code += 0x4000;
							obj_code += 0x00000FFF & (symbolAddress(word.operand1) - base);
						}
						else {
							strcpy(buffer, word.opcode);
							strcpy(word.opcode, "+");
							strcat(word.opcode, buffer);
							obj_code = opcodeValue(word.opcode);
						}
					}
					sprintf(buffer, "%06X", obj_code);
				}

				// FORMAT 4
				if (opcodeType(word.opcode) == 4) {
					obj_code *= 0x1000000;
					obj_code += 0x100000;
					if (strcmp(word.operand2, "X") == 0) {
						obj_code += 0x800000;
					}
					if (word.operand1[0] == '@') {
						// indirect addressing
						obj_code += 0x2000000;
						if (inSymbolTable(word.operand1)) {
							obj_code += symbolAddress(word.operand1);
							insert_modi_rec(current_addr + 1, 5);
						}
						else {
							set_error_flag(INVALID_OPERAND, line_num);
						}
					}
					else if (word.operand1[0] == '#') {
						// immediate addressing
						obj_code += 0x1000000;
						if (isDec(word.operand1 + 1)) {
							obj_code += strtol(word.operand1 + 1, &temp, 10);
						}
						else if (inSymbolTable(word.operand1)) {
							obj_code += symbolAddress(word.operand1);
							insert_modi_rec(current_addr + 1, 5);
						}
						else {
							set_error_flag(INVALID_OPERAND, line_num);
						}
					}
					else {
						obj_code += 0x3000000;
						obj_code += symbolAddress(word.operand1);
						insert_modi_rec(current_addr + 1, 5);
					}
					sprintf(buffer, "%08X", obj_code);
				}
			}

			// Byte
			else if (strcmp(word.opcode, "BYTE") == 0) {
				if (word.operand1[0] == 'C') {
					temp = strtok(word.operand1 + 1, "'");
					buffer[0] = '\0';
					for (i = 0; i < (int)strlen(temp); i++) {
						buffer[i] = temp[i];
					}
				}
				else if(word.operand1[0] == 'X') {
					temp = strtok(word.operand1 + 1, "'");
					if (isHex(temp)) {
						strcpy(buffer, temp);
					}
					else {
						set_error_flag(INVALID_OPERAND, line_num);
					}
				}
			}

			// Word
			else if (strcmp(word.opcode, "WORD") == 0) {
				if (isDec(word.operand1)) {
					sprintf(buffer, "%06X", strtol(word.operand1, &temp, 16));
				}
				else {
					set_error_flag(INVALID_OPERAND, line_num);
				}
			}
			
			// Base
			else if (strcmp(word.opcode, "BASE") == 0) {
				base = symbolAddress(word.operand1);
			}


			// wirte down on obj file
			// RESW, REWB
			else if (strcmp(word.opcode, "RESB") == 0 || strcmp(word.opcode, "RESW") == 0) {
				if (record_line[0] != '\0') {
					fprintf(obj_fp, "%02X", strlen(record_line) / 2);
					fprintf(obj_fp, "%s\n", record_line);
					record_line[0] = '\0';
				}
			}

			// new obj line
			if (record_line[0] == '\0' && strcmp("RESB", word.opcode) && strcmp("RESW", word.opcode)) {
				fprintf(obj_fp, "T%06X", current_addr);
			}

			// write obj_code
			if (obj_code) {
				fprintf(lst_fp, "%s\n", buffer);
				if ((strlen(record_line) + strlen(buffer)) / 2 > 0x1E) {
					fprintf(obj_fp, "%02X", strlen(record_line) / 2);
					fprintf(obj_fp, "%s\n", record_line);
					fprintf(obj_fp, "T%06X", current_addr);

					strcpy(record_line, buffer);
				}
				else {

					strcat(record_line, buffer);
				}
			}
			else {
				fprintf(lst_fp, "\n");
			}
		}

		obj_code = 0;
		fgets(line, 257, asm_fp);
		line_num++;
		if (!ignoredLine(line)) {
			word = parseLine(line);
		}
	}
}

int assemble(char** args){
	FILE* asm_fp;
	FILE* obj_fp;
	FILE* lst_fp;
	char lst_file[100];
	char obj_file[100];
	char temp[100];
	int prog_length;

	clear_symbol_table();
	clear_modification_record_table();
	reset_error_flag(flag);
	
	// check whether .asm file or not
	if (strcmp(args[1] + strlen(args[1] - 4), ".asm")) {
		set_error_flag(NOT_ASM_FILE, -1);
	}

	// set obj, lst temp file name
	strcpy(lst_file, args[1]);
	strcpy(obj_file, args[1]);
	lst_file[strlen(lst_file) - 3] = '\0';
	obj_file[strlen(obj_file) - 3] = '\0';
	strcat(lst_file, "lst.tmp");
	strcat(obj_file, "obj.tmp");

	// open files
	if (!(asm_fp = fopen(args[1], "r")) ||
		!(lst_fp = fopen(lst_file, "w")) ||
		!(obj_fp = fopen(obj_file, "w"))) {
		// fail to open file
		set_error_flag(FILE_OPEN_FAILURE, -1);
		return 0;
	}

	if(!check_error()) prog_length = assemble_pass1(asm_fp);
	rewind(asm_fp);
	if(!check_error()) assemble_pass2(asm_fp, obj_fp, lst_fp, prog_length);

	fclose(asm_fp);
	fclose(lst_fp);
	fclose(obj_fp);

	if (check_error()) {
		remove(lst_file);
		remove(obj_file);
		print_error();
	}
	else {
		strcpy(temp, lst_file);
		lst_file[strlen(lst_file) - 4] = '\0';
		remove(lst_file);
		rename(temp, lst_file);
		strcpy(temp, obj_file);
		obj_file[strlen(obj_file) - 4] = '\0';
		remove(obj_file);
		rename(temp, obj_file);
		printf("assemble: created file: %s, %s\n", obj_file, lst_file);
	}

	return 1;
}
