#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute(){
	swaddr_write(reg_l(R_DI),DATA_BYTE,swaddr_read(REG(R_EAX),DATA_BYTE));
	if(cpu.DF==0){
		reg_l(R_EDI)+=DATA_BYTE;
	}else{
		reg_l(R_EDI)-=DATA_BYTE;
	}
	//swaddr_write(reg_l(R_EDI),4,swaddr_read(reg_l(R_EAX),4));
	print_asm("stos");
}
make_instr_helper(n)
#include "cpu/exec/template-end.h"
