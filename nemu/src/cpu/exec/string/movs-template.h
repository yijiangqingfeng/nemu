#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute(){
	MEM_W(REG(R_EDI),MEM_R(REG(R_ESI)));
	if(cpu.DF==0){
		reg_l(R_EDI)+=DATA_BYTE;
		reg_l(R_ESI)+=DATA_BYTE;
	}else{
		reg_l(R_EDI)-=DATA_BYTE;
		reg_l(R_ESI)-=DATA_BYTE;
	}
	print_asm("movs");
}
make_instr_helper(n)
#include "cpu/exec/template-end.h"
