#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_n_,SUFFIX)){
	swaddr_write(reg_l(R_EDI),4,swaddr_read(reg_l(R_EAX),4));
	if(cpu.DF==0){
		reg_l(R_EDI)+=DATA_BYTE;
	}else{
		reg_l(R_EDI)-=DATA_BYTE;
	}
	//swaddr_write(reg_l(R_EDI),4,swaddr_read(reg_l(R_EAX),4));
	print_asm("stos");
	return 1;
}
//make_instr_helper(n)
#include "cpu/exec/template-end.h"
