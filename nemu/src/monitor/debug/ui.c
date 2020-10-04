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
	//if(args == NULL)return 0;
	printf("eax is %u\n",cpu.eax);
	printf("ecx is %u\n",cpu.ecx);
	printf("edx is %u\n",cpu.edx);
	printf("ebx is %u\n",cpu.ebx);
	printf("esp is %u\n",cpu.esp);
	printf("ebp is %u\n",cpu.ebp);
	printf("esi is %u\n",cpu.esi);
	printf("edi is %u\n",cpu.edi);
	return 0;
}

static int cmd_x(char *args);

static int cmd_x(char *args){
	char *args_1;
	args_1 = strtok(args," ");
	args = strtok(NULL," ");
	int x_tmp_1 = strlen(args_1);
	int x_cnt_1 = 0;
	int i_1;	
	for(i_1 = 0;i_1 < x_tmp_1;i_1++){
		x_cnt_1 = x_cnt_1 + (args_1[i_1]-'0');
		if(i_1!=x_tmp_1-1){x_cnt_1 = x_cnt_1 * 10;}
	}	
	int x_tmp_2 = strlen(args);
	int x_cnt_2 = 0;
	int i_2;	
	for(i_2 = 2;i_2 < x_tmp_2;i_2++){
		x_cnt_2 = x_cnt_2 + (args[i_2]-'0');
		if(i_2!=x_tmp_2-1){x_cnt_2 = x_cnt_2 * 16;}
	}
	uint32_t x_tmp;
	x_tmp = (uint32_t) x_cnt_2;
	int i;
	for(i = 0;i < x_cnt_1;i ++){
		printf("%x\n",swaddr_read(x_tmp+i*4,4));
	}
	return 0;
}

static int cmd_p(char* args);

static int cmd_p(char* args){
	cpu_exec(1);
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
