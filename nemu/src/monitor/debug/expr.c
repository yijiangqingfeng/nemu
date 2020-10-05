#include "nemu.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, 
	EQ,
	NUM,
	REG,
	HEX,
	POINTER,
	MINUS,
	NOT,
	AND,
	OR,
	NEQ,
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
	int priority;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{"\\(",'(',7},
	{"\\)",')',7},
	{"\\*",'*',4},
	{"/",'/',4},
	{"\\+",'+',3},					// plus
	{"\\-",'-',3},

	{" +",	NOTYPE,0},				// spaces
	{"==", EQ,2},						// equal
	{"\\b[0-9]+\\b",NUM,0},
	{"\\$[a-zA-Z]+",REG,0},
	{"\\b0[xX][0-9a-fA-F]+\\b",HEX,0},
	{"!=",NEQ,2},
	{"&&",AND,1},
	{"\\|\\|",OR,1},
	{"!",NOT,6},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int priority;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					case REG:
						tokens[nr_token].type = rules[i].token_type;
						tokens[nr_token].type = rules[i].priority;
						strncpy(tokens[nr_token].str,substr_start+1,substr_len-1);
						tokens[nr_token].str[substr_len-1]='\0';
						nr_token ++;
						break;
					default:
						tokens[nr_token].type = rules[i].token_type;
						tokens[nr_token].priority = rules[i].priority;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token ++;
				}

				break;
			}
		}
		
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}
int dominant_operator(int p,int q){
	int i,j;
	int min_p = 10;
	int oper = p;
	for(i = p;i <= q;i ++){
		if(tokens[i].type == NUM || tokens[i].type == REG || tokens[i].type == HEX)continue;
		int cnt = 0;
		bool flag = true;
		for(j = i - 1; j >= p;j --){
			if(tokens[j].type == '(' && !cnt){flag = false;break;}
			if(tokens[j].type == ')')cnt++;
			if(tokens[j].type == '(')cnt--;
		}
		if(!flag)continue; 
		if(tokens[i].priority<min_p){min_p = tokens[i].priority;oper=i;};
	}
	return oper;
}
bool check_parentheses(int p,int q){
	int i;
	if(tokens[p].type == '(' && tokens[q].type == ')'){
		int l = 0,r = 0;
		for(i = p;i <= q;i++){
			if(tokens[i].type == '(')l++;
			if(tokens[i].type == ')')r++;
			if(r>l)return false;
		}
		if(r==l)return true;
	}
	return false;
}
int eval(int p,int q){
	if(p>q){
		return 0;
	}else if(p==q){
		int tmp = strlen(tokens[q].str);
		printf("ha\n");
		if(tokens[q].type == NUM){
			int x_cnt_1 = 0;
			int i_1;	
			for(i_1 = 0;i_1 < tmp;i_1++){
				x_cnt_1 = x_cnt_1 + (tokens[q].str[i_1]-'0');
				if(i_1!=tmp-1){x_cnt_1 = x_cnt_1 * 10;}
			}
			return x_cnt_1;
		}else if(tokens[q].type == HEX){
			int x_cnt_2 = 0;
			int i_2;	
			for(i_2 = 2;i_2 < tmp;i_2++){
				x_cnt_2 = x_cnt_2 + (tokens[q].str[i_2]-'0');
				if(i_2!=tmp-1){x_cnt_2 = x_cnt_2 * 16;}
			}
			return x_cnt_2;
		}else if(tokens[q].type == REG){
			if(strlen(tokens[q].str) == 3){
				int k;
				for(k = R_EAX;k <= R_EDI;k++)if(strcmp(tokens[q].str , regsl[k]) == 0)break;
				return reg_l(k);
			}
			if(strlen(tokens[q].str) == 2){
				if(tokens[q].str[1] == 'X' ||tokens[q].str[1] == 'x' ||tokens[q].str[1] == 'p' ||tokens[q].str[1] == 'P' ||tokens[q].str[1] == 'I' ||tokens[q].str[1] == 'i'){
					int k;
					for(k = R_AX;k <= R_DI;k++)if(strcmp(tokens[q].str , regsw[k]) == 0)break;
					return reg_w(k);
				}
				if(tokens[q].str[1] == 'l' ||tokens[q].str[1] == 'L' ||tokens[q].str[1] == 'H' ||tokens[q].str[1] == 'h'){
					int k;
					for(k = R_AL;k <= R_BH;k++)if(strcmp(tokens[q].str , regsb[k]) == 0)break;
					return reg_b(k);
				}
			
			}
		}	
	}else if(check_parentheses(p,q) == true){
		return eval(p+1,q-1);
	}else {
		int op = dominant_operator(p,q);
		int val1 = eval(p,op-1);
		int val2 = eval(op+1,q);
		switch(tokens[op].type){
			case '+' :return val1 + val2;
			case '-' :return val1 - val2;
			case '*' :return val1 * val2;
			case '/' :return val1 / val2;
			case MINUS:return 0 - val2;
			case POINTER:return swaddr_read((uint32_t)val2,4);
			case EQ:return val1 == val2;
			case NEQ:return val1 != val2;
			case AND:return val1 && val2;
			case OR:return val1 || val2;
			case NOT:return !val2;
		}
	}
	return 10086;
}
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int i;
	/* TODO: Insert codes to evaluate the expression. */
	for(i = 0;i < nr_token;i ++){
		if(tokens[i].type == '*' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != ')' && tokens[i-1].type != REG && tokens[i-1].type != HEX )) ){
			tokens[i].type = POINTER;
			tokens[i].priority = 6;
		}
		if(tokens[i].type == '-' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != ')' && tokens[i-1].type != REG && tokens[i-1].type != HEX )) ){
			tokens[i].type = MINUS;
			tokens[i].priority = 5;
		}
		
	}
	*success = true;
	//panic("please implement me");
	return eval(0,nr_token-1);
}

