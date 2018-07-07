#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <dirent.h>
#include <sys/types.h>

int help(char** args){
	printf("\n");
	printf("\th[elp]\n");
	printf("\td[ir]\n");
	printf("\tq[uit]\n");
	printf("\thi[story]\n");
	printf("\tdu[mp][start, end]\n");
	printf("\te[dit] address, value\n");
	printf("\tf[ill] start, end, value\n");
	printf("\treset\n");
	printf("\topcode mnemonic\n");
	printf("\topcodelist\n");
	printf("\tassemble filename\n");
	printf("\ttype filename\n");
	printf("\tsymbol\n");
	printf("\n");

	return 1;
}

int dir(char** args){
	//DIR* dir;
	//struct dirent* entry = NULL;
	//
	//if((dir = opendir("./")) == NULL){
	//	fprintf(stderr, "dir: failed to open directory\n");
	//	return 0;
	//}

	//printf("\n");
	//while((entry = readdir(dir)) != NULL){
	//	if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
	//		printf("\t%s", entry->d_name);
	//		if(entry->d_type == DT_DIR){
	//			printf("/\n");
	//		}
	//		else if(!strcmp(".out", entry->d_name + strlen(entry->d_name) - 4)){
	//			printf("*\n");
	//		}
	//		else{
	//			printf("\n");
	//		}
	//	}
	//}
	//printf("\n");
	//closedir(dir);

	return 1;
}

int quit(char** args){
	exit(1);
}

int type(char** args) {
	FILE* fp;
	char line[257];

	if (!(fp = fopen(args[1], "r"))) {
		return 0;
	}
	while (1) {
		fgets(line, 256, fp);
		fprintf(stdout, "%s", line);
		if (feof(fp)) {
			break;
		}
	}

	fprintf(stdout, "\n");
	fclose(fp);

	return 1;
}