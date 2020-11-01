#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	if(op_src -> val > op_dest -> val){
		cpu.ZF=0;
		cpu.CF=1;
	}else if(op_src -> val == op_dest -> val){
		cpu.ZF=1;
		cpu.CF=0;
	}else{
		cpu.ZF=0;
		cpu.CF=0;
	}
	DATA_TYPE rst = op_dest -> val - op_src -> val;
	int len = (DATA_BYTE << 3) -1;
	cpu.SF = rst >> len;
	int s1,s2;
	s1 = op_dest -> val >> len;
	s2 = op_src -> val >> len;
	cpu.OF = (s1!=s2 && s2 == cpu.SF);
	rst ^= rst >> 4;
	rst ^= rst >> 2;
	rst ^= rst >> 1;
	cpu.PF = !(rst & 1);
	print_asm_no_template2();
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
