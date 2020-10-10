#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char expr[32];
	int b;
	int val;

} WP;
WP* new_wp();
void free_wp(WP* wp);
void info_wp();
void delete_wp();
bool check();
#endif
