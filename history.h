
#ifndef _HISTORY_H_
#define _HISTORY_H_

typedef struct history_list* history_ptr;
typedef struct history_list{
	char line[257];
	history_ptr next;
}history_list;

int history(char**);
void tempHistory(char*);
void addHistory(void);

extern history_ptr history_head;
extern history_ptr temp_history;

#endif
