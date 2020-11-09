#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	if(DATA_BYTE == 2){
		reg_l (R_ESP) -= 2;
		MEM_W(reg_l(R_ESP) ,op_src->val);
	}else{
		reg_l (R_ESP) -= 4;
		if(DATA_BYTE == 1)op_src->val = (uint8_t) op_src->val;
	swaddr_write(reg_l(R_ESP),4,op_src->val);
	}
	print_asm_no_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
