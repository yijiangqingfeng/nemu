#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
	if(op_src -> size == 1 && op_dest -> size != 1)
	op_src -> val = (int8_t) op_src -> val;
	DATA_TYPE rst = op_dest -> val - op_src -> val;
	int len = (DATA_BYTE << 3) -1;
	cpu.CF = op_dest -> val < op_src -> val;
	cpu.SF = rst >> len;
	int s1,s2;
	s1 = op_dest -> val >> len;
	s2 = op_src -> val >> len;
	cpu.OF = (s1!=s2 && s2 == cpu.SF);
	cpu.ZF = !rst;
	OPERAND_W(op_dest,rst);
	rst ^= rst >> 4;
	rst ^= rst >> 2;
	rst ^= rst >> 1;
	cpu.PF = !(rst & 1);
	print_asm_no_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
