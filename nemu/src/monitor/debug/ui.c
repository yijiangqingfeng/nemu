#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_si(char *args){
	if(args == NULL){
		cpu_exec(1);
		return 0;
	}	
	int si_tmp = strlen(args);
	int si_cnt = 0;
	int i;	
	for(i = 0;i < si_tmp;i++){
		si_cnt = si_cnt + (args[i]-'0');
		if(i!=si_tmp-1){si_cnt = si_cnt * 10;}
	}
	while(si_cnt--)cpu_exec(1);
	return 0;
}

static int cmd_info(char *args);

static int cmd_info(char *args){
	if(args==NULL)return 0;
	if(args[0] == 'r'){
		printf("eax is 0x%x\n",cpu.eax);
		printf("ecx is 0x%x\n",cpu.ecx);
		printf("edx is 0x%x\n",cpu.edx);
		printf("ebx is 0x%x\n",cpu.ebx);
		printf("esp is 0x%x\n",cpu.esp);
		printf("ebp is 0x%x\n",cpu.ebp);
		printf("esi is 0x%x\n",cpu.esi);
		printf("edi is 0x%x\n",cpu.edi);
	}
	else if(args[0] == 'w'){
		info_wp();		
	}
	return 0;
}

static int cmd_x(char *args);

static int cmd_x(char *args){
	if(args==NULL)return 0;
	char *args_1;
	args_1 = strtok(args," ");
	if(args_1==NULL)return 0;
	args = strtok(NULL," ");
	if(args==NULL)return 0;
	uint32_t x_cnt_1;
	bool flag1;
	x_cnt_1 = expr(args_1, &flag1);

	uint32_t x_cnt_2;
	bool flag2;
	x_cnt_2 = expr(args, &flag2);
	
	int i;
	for(i = 0;i < x_cnt_1;i ++){
		printf("%x\n",swaddr_read(x_cnt_2+i*4,4));
	}
	return 0;
}

static int cmd_p(char* args);

static int cmd_p(char* args){
	if(args==NULL)return 0;
	char *args_1;
	args_1 = strtok(args," ");
	if(args_1==NULL)return 0;
	args = strtok(NULL," ");
	if(args==NULL)return 0;
	uint32_t n;
	bool flag;
	n = expr(args, &flag);
	printf("%d\n",n);
	return 0;
}

static int cmd_w(char* args);

static int cmd_w(char* args){
	if(args==NULL)return 0;
	char *args_1;
	args_1 = strtok(args," ");
	if(args_1==NULL)return 0;
	args = strtok(NULL," ");
	if(args==NULL)return 0;
	WP* p;
	bool suc;
	p = new_wp();
	printf("Watchpoint %d: %s\n",p -> NO,args);
	p -> val = expr(args,&suc);
	strcpy(p -> expr,args);
	printf("Value : %d\n",p -> val);
	return 0;
}

static int cmd_d(char* args);

static int cmd_d(char* args){
	if(args==NULL)return 0;
	char *args_1;
	args_1 = strtok(args," ");
	if(args_1==NULL)return 0;
	args = strtok(NULL," ");
	if(args==NULL)return 0;
	int num;
	sscanf(args,"%d",&num);
	delete_wp(num);
	return 0;
}
static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
	{"si","si command.",cmd_si },
	{"info","info command.",cmd_info },
	{"x","x command.",cmd_x },
	{"p","p command.",cmd_p},
	{"w","w command.",cmd_w},
	{"d","d command.",cmd_d},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
