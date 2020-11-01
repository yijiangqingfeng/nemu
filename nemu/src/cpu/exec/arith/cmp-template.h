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
	print_asm_template2();
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
