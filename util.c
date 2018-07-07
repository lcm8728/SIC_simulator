#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isHex(char* string) {
	if (strspn(string, "0123456789abcdefABCDEF") == strlen(string)) return 1;
	return 0;
}

int isDec(char* string) {
	if (strspn(string, "0123456789") == strlen(string)) return 1;
	return 0;
}

int isEmptyLine(char* string) {
	if (strspn(string, " \t\n") == strlen(string)) return 1;
	return 0;
}