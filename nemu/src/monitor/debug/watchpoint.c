#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
	WP* new_watchpoint;
	WP* p;
	new_watchpoint = free_;
	free_ = free_ -> next;
	new_watchpoint -> next = NULL;
	p = head;
	if(p == NULL){
		head = new_watchpoint;
		p = head;
	}else{
		while(p -> next != NULL)p = p -> next;
		p -> next = new_watchpoint;
	}
	return new_watchpoint;
}
void free_wp(WP* wp){
	WP* f;
	WP* h;
	f = free_;
	if(f == NULL)free_ = wp;
	else{
		wp -> next = free_;
		free_ = wp;
	}
	h = head;
	if(head == NULL)assert(0);
	if(head -> NO == wp -> NO){
		head = head -> next;
	}else{
		while(h -> next != NULL && h -> next -> NO != wp -> NO)h = h -> next;
		h -> next = h -> next -> next;
	}
	wp -> next = NULL;
}





