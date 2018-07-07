#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

history_ptr history_head = NULL;
history_ptr temp_history = NULL;

int history(char** args){
	int history_num = 1;
	history_ptr next = history_head;
	while(next){
		printf("\t%d\t%s", history_num++, next->line);
		next = next->next;
	}

	return 1;
}

void tempHistory(char* line){
	temp_history = (history_list*)malloc(sizeof(history_list));
	strcpy(temp_history->line, line);
	temp_history->next = NULL;
}

void addHistory(){
	history_ptr temp = history_head;

	if(temp == NULL){
		history_head = temp_history;
		temp_history = NULL;
		return;
	}

	while(temp->next){
		temp = temp->next;
	}

	temp->next = temp_history;
	temp_history = NULL;
}
