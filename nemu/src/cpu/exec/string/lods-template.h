#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
	if(ops_decoded.is_stack_size_16){
		reg_l(R_AL) = swaddr_read(reg_l(R_SI),4);
		if(cpu.DF==0){
			reg_w(R_SI)+=DATA_BYTE;
		}else{
			reg_w(R_SI)-=DATA_BYTE;
		}
	}else{
		reg_l(R_AL) = swaddr_read(reg_l(R_ESI),4);
		if(cpu.DF==0){
			reg_l(R_ESI)+=DATA_BYTE;
		}else{
			reg_l(R_ESI)-=DATA_BYTE;
		}
	}
	print_asm("lods");
}
make_instr_helper(n)
#include "cpu/exec/template-end.h"
