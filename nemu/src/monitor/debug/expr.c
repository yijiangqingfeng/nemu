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
	{"\\*",'*',6},
	{"/",'/',6},
	{"\\+",'+',5},					// plus
	{"\\-",'-',5},

	{" +",	NOTYPE,0},				// spaces
	{"==", EQ,3},						// equal
	{"[0-9]+",NUM,0}
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
					default:
						tokens[nr_token].type = rules[i].token_type;
						tokens[nr_token].priority = rules[i].priority;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						nr_token ++;
						tokens[nr_token].str[substr_len]='\0';
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
		if(tokens[i].type == NUM)continue;
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
			if(r>=l)return false;
		}
		if(r==l)return true;
	}
	return false;
}
int eval(int p,int q){
	if(p>q){
		return 0;
	}else if(p==q){
		int x_tmp_1 = strlen(tokens[q].str);
		int x_cnt_1 = 0;
		int i_1;	
		for(i_1 = 0;i_1 < x_tmp_1;i_1++){
			x_cnt_1 = x_cnt_1 + (tokens[q].str[i_1]-'0');
			if(i_1!=x_tmp_1-1){x_cnt_1 = x_cnt_1 * 10;}
			}
		return x_cnt_1;
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
		}
	}
	return 10086;
}
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	
	//panic("please implement me");
	return eval(0,nr_token-1);
}

